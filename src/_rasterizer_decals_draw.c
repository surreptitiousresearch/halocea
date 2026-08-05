/* _rasterizer_decals_draw 0x83711270 — draw every decal chained off one cluster's current layer. Walks the
 * per-cluster decal list (head from decal_get_first_decal_index, chained through decal+52), and for each decal:
 * reprograms the framebuffer blend + color-write mask when the definition's blend function changes, binds the
 * decal bitmap (cached), uploads the per-decal modulation color into vertex-shader constant register 10, then
 * draws its cached triangle batch out of the shared decal vertex buffer. Runs only outside special drawing
 * modes, and only on layer 3 when environment decals are globally disabled.
 *
 * DEVIATION 1 (decompiler "local variable allocation has failed"): the per-decal color upload was emitted as
 * raw inline GPU pokes into global_d3d_device->m_Constants.Alu[10].vector4_f32[0..3] (device offsets
 * 0x820/0x824/0x828/0x82C) followed by m_Pending.m_Mask[0] |= (1<<61). That is the inline expansion of writing
 * one float4 to vertex-shader constant register 10; restored to D3DDevice_SetVertexShaderConstantFN (reg 0xA,
 * count 1, PendingMask0 = (uint64)1<<61), matching the sibling light/fog begins. The decompiler also rendered
 * the mask bit as the 32-bit __ROR4__(1,3)=0x20000000; the std/ld are 64-bit so the real bit is 1<<61.
 *
 * DEVIATION 2: the color source decal[+0x24] is a packed D3DCOLOR ARGB (byte3=A, byte2=R, byte1=G, byte0=B).
 * The disasm maps the four extracted bytes to const-register channels as (R, G, B, 255-alpha) each /255, where
 * alpha = (decal_fade * A + 127) >> 8. The decompiler's OVERLAPPED __int64 byte-extraction obscured this; the
 * register trace (extrwi/clrlwi/subfic → fcfid → stfs) gives the channel order used here.
 *
 * Decal records are typed as decal_datum (56-byte DB struct): definition_index@44, color@36, intensity@40,
 * quad_count@42, bitmap_index@27, next_decal_index@52. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/decal_datum.h"
#include "headers/decal_definition.h"
#include "headers/decal_layer.h"
#include "headers/shader_framebuffer_blend_function.h"
#include "headers/global_tag_instances.h"
#include "headers/lruv_cache.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/point2d.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_vertex_shader_index.h"



#include "headers/point2d.h"
extern int16_t rasterizer_decal_cached_bitmap_index;       /* cached decal bitmap_index */
extern int rasterizer_decal_cached_bitmap_group_index;          /* cached decal bitmap_group_index */

extern int decal_get_first_decal_index(int16_t cluster_index, int16_t layer);
extern unsigned int lruv_block_get_address(const lruv_cache *cache, uint16_t block_index);
extern point2d *rasterizer_set_texture(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index);
extern void rasterizer_set_framebuffer_blend_function(int16_t framebuffer_blend_function);
extern void set_decal_shader(int16_t local_framebuffer_blend_function);

extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);

void _rasterizer_decals_draw(int16_t cluster_index)
{
    bool enabled;
    int decal_index;

    enabled = 1;
    if ( !rasterizer_debug_options.draw_environment_decals )
        enabled = (local_layer == _decal_layer_alpha_tested);
    if ( rasterizer_debug_options.drawing_mode || !enabled )
        return;

    decal_index = decal_get_first_decal_index(cluster_index, local_layer);
    if ( decal_index == -1 )
        return;

    do
    {
        decal_datum *decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, decal_index);
        decal_definition *definition = TAG_GET(decal_definition, decal->definition_index);
        int16_t blend_function = definition->shader.decal.framebuffer_blend_function;

        /* reprogram framebuffer blend + color-write mask when the blend function changes */
        if ( local_framebuffer_blend_function != blend_function )
        {
            local_framebuffer_blend_function = blend_function;
            D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device,
                (blend_function == _shader_framebuffer_blend_function_multiply
                 || blend_function == _shader_framebuffer_blend_function_double_multiply) ? 15 : 7);
            rasterizer_set_framebuffer_blend_function(local_framebuffer_blend_function);
        }

        int bitmap_group_index = definition->shader.decal.map.index;
        int16_t bitmap_index = (signed char)decal->bitmap_index;
        unsigned int decal_color = decal->color;
        unsigned int decal_fade = decal->intensity;
        int triangle_count = 2 * decal->quad_count;
        int vertex_offset = (int)((double)(lruv_block_get_address(local_vertex_cache, decal_index) >> 4) * 1.5);

        /* bind the decal bitmap (cached against the last-set group/index) */
        if ( rasterizer_decal_cached_bitmap_group_index != bitmap_group_index || rasterizer_decal_cached_bitmap_index != bitmap_index )
        {
            rasterizer_decal_cached_bitmap_group_index = bitmap_group_index;
            rasterizer_decal_cached_bitmap_index = bitmap_index;
            rasterizer_set_texture(0, 0, 1, bitmap_group_index, bitmap_index);
        }

        /* per-decal modulation color → vertex-shader constant register 10 (R,G,B, 255-alpha; each /255) */
        unsigned int modulated_alpha = (decal_fade * (decal_color >> 24) + 127) >> 8;
        float decal_color_constant[4];
        decal_color_constant[0] = (float)((decal_color >> 16) & 0xFF) * 0.00392156862745098;
        decal_color_constant[1] = (float)((decal_color >> 8) & 0xFF) * 0.00392156862745098;
        decal_color_constant[2] = (float)(decal_color & 0xFF) * 0.00392156862745098;
        decal_color_constant[3] = (float)(255 - modulated_alpha) * 0.00392156862745098;
        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, decal_color_constant, 1,
                                            (uint64_t)1 << 61);

        D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_decal));
        D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_decal));
        set_decal_shader(local_framebuffer_blend_function);
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLELIST, 3 * triangle_count,
                                 (char *)local_d3d_vertex_buffer + 16 * vertex_offset, 0x10);

        decal_index = decal->next_decal_index;
    }
    while ( decal_index != -1 );
}
