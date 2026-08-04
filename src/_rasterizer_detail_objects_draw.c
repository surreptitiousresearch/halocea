/* _rasterizer_detail_objects_draw @0x837133A8 — draw one frame's worth of screen-facing detail-object
 * billboards (grass, rocks, etc.), one D3D batch per detail-object layer. For each layer: resolves the
 * layer's collection definition and its sprite-sheet bitmap group tag, binds the bitmap to stage 0, builds
 * a per-sprite-type UV/scale constant block (register 0x13) from the collection definition's sprite
 * property table, builds a per-sprite-instance placement constant block (register 0x1D) from the bitmap
 * group's per-sequence sprite rectangles, uploads both, sets the detail-object vertex declaration/shader,
 * then draws each cell's vertex range as a triangle list.
 *
 * DEVIATION: kept the decompiler's raw tag-data pointer indexing (as in the sibling detail-object
 * functions) rather than inventing named structs for the collection-definition/bitmap-group internals —
 * the DB does not resolve their layouts, only the byte offsets actually touched here.
 * FAITHFUL: both SetVertexShaderConstantFN PendingMask0 values are runtime-count-dependent — reproduces
 * the same "build a run of set bits via a signed shift of a single top bit, then reposition" trick as
 * _rasterizer_model_setupnodeparts (same mask_seed = 1ULL<<63, different additive constants/final shift). */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/detail_object_global_runtime_data.h"
#include "headers/scenario_detail_object_collection_palette_entry.h"
#include "headers/detail_object_collection_definition.h"
#include "headers/detail_object_type_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_data.h"
#include "headers/d3d_render_boundary.h"
#include "headers/point2d.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
typedef struct D3DVertexShader D3DVertexShader;
#include "headers/point2d.h"
extern void D3DDevice_DrawVertices(D3DDevice *pDevice, _D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int VertexCount);

extern int16_t main_get_window_count(void);

extern point2d *rasterizer_set_texture(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *pDevice, D3DVertexDeclaration *pDecl);
extern void D3DDevice_SetVertexShader(D3DDevice *pDevice, D3DVertexShader *pShader);

void _rasterizer_detail_objects_draw(const detail_object_view_data *detail_object_view_data)
{
    if (!rasterizer_debug_options.draw_detail_objects || main_get_window_count() > 1)
        return;

    scenario *scen = global_scenario;
    if (detail_object_view_data->layer_count <= 0)
        return;

    for (__int16 layer_index = 0; layer_index < detail_object_view_data->layer_count; ++layer_index)
    {
        detail_object_layer_data *layer = &detail_object_view_data->layers[layer_index];

        const detail_object_collection_definition *collection_def = TAG_GET(const detail_object_collection_definition, ((scenario_detail_object_collection_palette_entry *)scen->detail_object_collection_palette.address) [layer->collection_definition_index].reference.index);

        int               bitmap_group_index = collection_def->map.index;
        const bitmap_group *bitmap_group_def = TAG_GET(const bitmap_group, bitmap_group_index);

        rasterizer_set_texture(0, 0, 1, bitmap_group_index, 0);

        /* register 0x13: per-sprite-type UV scale/offset (4 floats each), from the collection def's
         * sprite property table (96-byte detail_object_type_definition records) */
        float sprite_type_constants[64];
        int   sprite_type_count = collection_def->type_definitions.count;
        if (sprite_type_count > 0)
        {
            const detail_object_type_definition *records =
                    (const detail_object_type_definition *)collection_def->type_definitions.address;
            for (int i = 0; i < sprite_type_count; ++i)
            {
                const detail_object_type_definition *record = &records[i];
                float u1 = record->far_fade_distance;
                float size_min = record->size_min;
                float u_span = record->far_fade_distance - record->near_fade_distance;
                float inv_u_span = (u_span > 0.0f) ? (1.0f / u_span) : u_span;

                /* sequence_index selects the sprite-sheet sequence; first_bitmap_index of that sequence
                 * then selects the 48-byte bitmap_data whose width/height feed the two fields below. */
                unsigned __int8 type_byte = record->sequence_index;
                __int16 frame_index = ((const bitmap_group_sequence *)bitmap_group_def->sequences.address)[type_byte].first_bitmap_index;
                const bitmap_data *frame_bitmap = &((const bitmap_data *)bitmap_group_def->bitmaps.address)[frame_index];

                float *out = &sprite_type_constants[4 * i];
                out[0] = u1 * inv_u_span;
                out[1] = -inv_u_span;
                out[2] = (float)frame_bitmap->width * size_min;
                out[3] = (float)frame_bitmap->height * size_min;
            }
        }

        /* register 0x1D: per-sprite-instance placement rectangle (4 floats each), from the bitmap group's
         * per-sequence sprite lists (64-byte bitmap_group_sequence records) */
        float sprite_instance_constants[514];
        int   sprite_instance_count = 0;
        int   sequence_count = bitmap_group_def->sequences.count;
        if (sequence_count > 0)
        {
            const bitmap_group_sequence *sequences = (const bitmap_group_sequence *)bitmap_group_def->sequences.address;
            for (int s = 0; s < sequence_count; ++s)
            {
                const bitmap_group_sequence *seq = &sequences[s];
                int sprite_count_in_sequence = seq->sprites.count;
                if (sprite_count_in_sequence > 0)
                {
                    const bitmap_group_sprite *sprites = (const bitmap_group_sprite *)seq->sprites.address;
                    for (int k = 0; k < sprite_count_in_sequence; ++k)
                    {
                        const bitmap_group_sprite *sprite = &sprites[k];
                        float                     *out    = &sprite_instance_constants[4 * sprite_instance_count++];
                        out[0] = sprite->bounds.n[0];
                        out[1] = sprite->bounds.n[2];
                        out[2] = sprite->bounds.n[1] - sprite->bounds.n[0];
                        out[3] = sprite->bounds.n[3] - sprite->bounds.n[2];
                    }
                }
            }
        }

        /* dynamic PendingMask0: a run of set bits built from a signed shift of a single top bit */
        __int64 mask_seed = (__int64)((unsigned __int64)1 << 63);

        unsigned int shift1 = ((unsigned int)(sprite_type_count + 18) >> 2) - 4;
        unsigned __int64 pending_mask1 = (unsigned __int64)(mask_seed >> shift1) >> 4;
        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x13, sprite_type_constants,
                                            sprite_type_count, pending_mask1);

        unsigned int shift2 = ((unsigned int)(sprite_instance_count + 28) >> 2) - 7;
        unsigned __int64 pending_mask2 = (unsigned __int64)(mask_seed >> shift2) >> 7;
        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x1D, sprite_instance_constants,
                                            sprite_instance_count, pending_mask2);

        D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_detail_object));
        D3DDevice_SetVertexShader(global_d3d_device,
                rasterizer_dx9_shaders_vshader9_get(collection_def->collection_type + 3));

        for (__int16 cell_index = 0; cell_index < layer->cell_count; ++cell_index)
        {
            detail_object_cell_data *cell = &layer->cells[cell_index];
            D3DDevice_DrawVertices(global_d3d_device, D3DPT_TRIANGLELIST,
                    cell->internal__first_vertex_index, 6 * cell->detail_object_count);
        }
    }
}
