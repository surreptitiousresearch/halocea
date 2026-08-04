/* rasterizer_set_texture_bitmap_data_for_effect @0x8369C528 — bind a bitmap as the texture for a given
 * sampler stage of an effect shader. A null bitmap is a no-op (returns 0). When the global "use white
 * texture" debug override is set, the flat white default is bound. Otherwise the bitmap's hardware texture
 * is ensured resident and bound; if it has no hardware format yet, the bitmap's default hardware format is
 * bound instead.
 *
 * DEVIATION: the decompiler lost the effect/vtable pointer in the no-hardware-format branch and emitted an
 * indirect call through uninitialized registers; both branches actually call ID3DXEffect_SetTexture with the
 * same parameter handle and differ only in the texture, restored from the disassembly. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_globals.h"
#include "headers/bitmap_data.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern char *tag_get_name(int16_t tag_index);
extern D3DBaseTexture *rasterizer_bitmap_get_hw(bitmap_data *bitmap, const char *name);
extern void *rasterizer_get_bitmap_default_hardware_format(const bitmap_data *bitmap);

int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap,
                                                  rasterizer_dx9_shader *dxeffect_shader)
{
    if (!bitmap)
        return 0;

    if (halo_use_white_tex)
    {
        ID3DXEffect_SetTexture(dxeffect_shader->effect, dxeffect_shader->texture[stage],
            (D3DBaseTexture *)rasterizer_globals.default_white_hardware_format);
        return 1;
    }

    _texture_cache_bitmap_get_hardware_format(bitmap, 1, 1);
    if (bitmap->hardware_format)
    {
        const char *name = tag_get_name(bitmap->tag_index);
        D3DBaseTexture *hardware_texture = rasterizer_bitmap_get_hw(bitmap, name);
        ID3DXEffect_SetTexture(dxeffect_shader->effect, dxeffect_shader->texture[stage], hardware_texture);
    }
    else
    {
        void *default_format = rasterizer_get_bitmap_default_hardware_format(bitmap);
        ID3DXEffect_SetTexture(dxeffect_shader->effect, dxeffect_shader->texture[stage],
            (D3DBaseTexture *)default_format);
    }
    return 1;
}
