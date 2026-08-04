/* render_model_parts @0x83798790 — draws every renderable part of a model, one region/permutation
 * choice per region, across up to 3 passes: opaque/shadow (pass 0), radiosity-flagged shaders
 * (pass 1), and transparent (pass 2). `flags` bit 1 selects single-pass (opaque/shadow only, via
 * rasterizer_environment_shadow_model_draw) vs. full 3-pass draw; bit 0 suppresses recording
 * transparent-part draw-order links.
 *
 * DEVIATION: `region_permutation_indices[region]`, `model_region`/`model_region_permutation`, and
 * `model_geometry`/`model_shader_reference` field accesses were cross-checked against
 * disasm_range(0x83798790, 0x83798824) (raw byte-offset math matched exactly against the DB's
 * types_members layouts for those types — model_region.permutations@0x40, .geometry_indices@0x40
 * within model_region_permutation, model_geometry.parts@0x24, model_shader_reference.shader.index@0xC
 * and .permutation_index@0x10) rather than trusted from decompiler output alone, since none of those
 * types existed as named C structs before this pass.
 *
 * The transparent pass reuses the same `sort_scratch` real_point3d[58] scratch buffer for two purposes: sort_scratch[0]
 * (bytes 0-12) holds matrix4x3_transform_point's per-part output; starting at byte 24, the same buffer
 * is reinterpreted as `render_sort_filth records[32]` — confirmed field-for-field via
 * disasm_range(0x83798AD8, 0x83798B70) (group_index@+8, next_part_index@+10, part_index@+12 all
 * matched the existing render_sort_filth.h layout once the base offset (+24) was found).
 *
 * FAITHFUL QUIRK: `records[transparent_link_count].group_index != 0xFFFF` is a guard read from this
 * scratch buffer, but nothing in this function ever writes `group_index` — it reads whatever the stack
 * happened to hold at that offset. Reproduced verbatim rather than guessing an initializer.
 *
 * UNRESOLVED: the `render_sort_filth` passed as rasterizer_model_transparent_geometry_submit's last
 * argument (a single struct, separate from the `records[]` table above) has no visible initializer in
 * this function's disasm — its lifecycle is presumably internal to that callee, which is out of scope
 * here. Declared as an uninitialized local matching the decompiler's own inability to resolve it. */

#include <stdint.h>
#include "headers/model.h"
#include "headers/model_region.h"
#include "headers/model_region_permutation.h"
#include "headers/model_geometry.h"
#include "headers/model_shader_reference.h"
#include "headers/gbxmodel_geometry_part.h"
#include "headers/model_part_flags.h"
#include "headers/render_skinning.h"
#include "headers/shader.h"
#include "headers/render_sort_filth.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/shader_type.h"
#include "headers/shader_model_flags.h"

#include "headers/real_matrix4x3.h"
extern uint8_t shader_type_is_valid_for_model(int16_t shader_type);
extern uint8_t shader_type_is_transparent(int16_t shader_type);
extern void rasterizer_model_setupnodeparts(int num_nodes, const uint8_t *node_table, real_matrix4x3 *node_matrices);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void rasterizer_model_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index, const real_point3d *centroid, render_sort_filth *sort_filth);
extern void rasterizer_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);
extern void rasterizer_environment_shadow_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, const vertex_buffer *vertex_buffer);
extern void rasterizer_debug_model_vertices(int target_object_index, const render_skinning *skinning, const gbxmodel_geometry_part *part);

void render_model_parts(
        const model *model,
        const char *region_permutation_indices,
        const render_skinning *skinning,
        int object_index,
        int16_t geometry_detail_level_index,
        int16_t forced_shader_permutation_index,
        int flags)
{
    int max_pass_index = ~flags & 2;                        /* 2 for a full 3-pass draw, 0 for opaque-only */
    unsigned char suppress_transparent_links = flags & 1;

    real_point3d sort_scratch[58];
    /* records overlay the local scratch at byte +24 (== &sort_scratch[2], real_point3d is 12 bytes) */
    render_sort_filth *records = (render_sort_filth *)&sort_scratch[2];

    for (int pass = 0; pass <= max_pass_index; pass = (__int16)(pass + 1))
    {
        int transparent_link_count = 0;

        if (model->regions.count > 0)
        {
            for (int region_index = 0; region_index < model->regions.count; region_index = (__int16)(region_index + 1))
            {
                char permutation_index = region_permutation_indices[region_index];
                if (permutation_index == -1)
                    continue;

                const model_region *region = (const model_region *)model->regions.address + region_index;
                const model_region_permutation *permutation =
                        (const model_region_permutation *)region->permutations.address + permutation_index;
                __int16 geometry_index = permutation->geometry_indices[geometry_detail_level_index];
                if (geometry_index == -1)
                    continue;

                const model_geometry *geometry = (const model_geometry *)model->geometries.address + geometry_index;
                if (geometry->parts.count <= 0)
                    continue;

                const gbxmodel_geometry_part *parts = (const gbxmodel_geometry_part *)geometry->parts.address;
                for (int part_index = 0; part_index < geometry->parts.count; part_index = (__int16)(part_index + 1))
                {
                    const gbxmodel_geometry_part *part = &parts[part_index];
                    const model_shader_reference *shader_ref =
                            (const model_shader_reference *)model->shaders.address + part->shader_index;
                    const shader *part_shader = TAG_GET(const shader, shader_ref->shader.index);

                    if (!shader_type_is_valid_for_model(part_shader->base.type) || (part->flags & (1u << _model_part_stripped_bit)) != 0)
                        continue;

                    if (part->flags & (1u << _model_part_local_nodes))
                        rasterizer_model_setupnodeparts(part->num_nodes, part->local_node_table, skinning->node_matrices);

                    if (shader_type_is_transparent(part_shader->base.type))
                    {
                        if (pass == 2)
                        {
                            matrix4x3_transform_point(&skinning->node_matrices[part->centroid_primary_node_index],
                                                       &part->centroid, sort_scratch);

                            __int16 shader_permutation = forced_shader_permutation_index;
                            if (!forced_shader_permutation_index)
                                shader_permutation = shader_ref->permutation_index;

                            render_sort_filth transparent_submit_scratch; /* scratch buffer reuse — see file header note */
                            rasterizer_model_transparent_geometry_submit(part_shader, shader_permutation,
                                    &part->triangle_buffer, -1, part->triangle_buffer.count, &part->vertex_buffer,
                                    -1, sort_scratch, &transparent_submit_scratch);

                            if (transparent_link_count < 32
                                && records[transparent_link_count].group_index != (short)0xFFFF /* FAITHFUL QUIRK */
                                && !suppress_transparent_links
                                && (part->next_part_index > 0 || part->prev_part_index > 0))
                            {
                                records[transparent_link_count].part_index = part_index;
                                records[transparent_link_count].next_part_index = part->next_part_index;
                                ++transparent_link_count;
                            }
                        }
                    }
                    else if (part_shader->base.type == _shader_type_model
                             && ((part_shader + 1)->base.radiosity.flags & (1u << _shader_model_alpha_blended_decal_bit)))
                    {
                        if (pass == 1)
                        {
                            __int16 shader_permutation = forced_shader_permutation_index;
                            if (!forced_shader_permutation_index)
                                shader_permutation = shader_ref->permutation_index;
                            rasterizer_model_draw(part_shader, shader_permutation, &part->triangle_buffer, -1,
                                                   part->triangle_buffer.count, &part->vertex_buffer, -1);
                        }
                    }
                    else if (pass == 0)
                    {
                        __int16 shader_permutation = forced_shader_permutation_index;
                        if (!forced_shader_permutation_index)
                            shader_permutation = shader_ref->permutation_index;

                        if (flags & 2)
                        {
                            rasterizer_environment_shadow_model_draw(part_shader, shader_permutation,
                                    &part->triangle_buffer, &part->vertex_buffer);
                        }
                        else
                        {
                            rasterizer_model_draw(part_shader, shader_permutation, &part->triangle_buffer, -1,
                                                   part->triangle_buffer.count, &part->vertex_buffer, -1);
                            rasterizer_debug_model_vertices(object_index, skinning, part);
                        }
                    }
                }
            }
        }

        for (int outer = 0; outer < transparent_link_count; outer = (__int16)(outer + 1))
        {
            int inner;
            for (inner = 0; inner < transparent_link_count; inner = (__int16)(inner + 1))
            {
                if (records[outer].next_part_index == records[inner].part_index && records[outer].next_part_index > 0)
                    break;
            }
            if (inner < transparent_link_count)
            {
                *records[outer].next_group_presorted_index_reference = records[inner].group_index;
                *records[inner].prev_group_presorted_index_reference = records[outer].group_index;
            }
        }
    }
}
