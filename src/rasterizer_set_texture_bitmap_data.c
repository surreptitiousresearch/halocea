/* rasterizer_set_texture_bitmap_data @0x8369C410 — bind a bitmap as the D3D texture for a given fixed-function
 * sampler stage. A null bitmap is a no-op (returns 0). When the global "use white texture" debug override is
 * set, the flat white default is bound. Otherwise the bitmap's hardware texture is ensured resident and
 * bound; if it has no hardware format yet, the bitmap's default hardware format is bound instead.
 *
 * DEVIATION: the decompiler's PendingMask3 (LODWORD/HIDWORD of v4/v7/v9) is garbage — traced from disasm,
 * all three call sites build it identically as (1ULL << 63) >> (stage + 32), i.e. 1ULL << (31 - stage): a
 * one-hot bit per texture sampler stage, offset +32 from the vertex/pixel-shader-constant pending masks used
 * elsewhere (see PendingMask0 in _rasterizer_model_setupnodeparts / _rasterizer_detail_objects_draw). */

#include <stdint.h>
#include "headers/rasterizer_globals.h"
#include "headers/bitmap_data.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern char *tag_get_name(int tag_index);
extern D3DBaseTexture *rasterizer_bitmap_get_hw(bitmap_data *bitmap, const char *name);
extern void *rasterizer_get_bitmap_default_hardware_format(const bitmap_data *bitmap);

int rasterizer_set_texture_bitmap_data(int16_t stage, bitmap_data *bitmap)
{
    if (!bitmap)
        return 0;

    uint64_t pending_mask = 1ULL << (31 - stage);

    if (halo_use_white_tex)
    {
        D3DDevice_SetTexture(global_d3d_device, stage,
            (D3DBaseTexture *)rasterizer_globals.default_white_hardware_format, pending_mask);
        return 1;
    }

    _texture_cache_bitmap_get_hardware_format(bitmap, 1, 1);
    if (bitmap->hardware_format)
    {
        const char *name = tag_get_name(bitmap->tag_index);
        D3DBaseTexture *hardware_texture = rasterizer_bitmap_get_hw(bitmap, name);
        D3DDevice_SetTexture(global_d3d_device, stage, hardware_texture, pending_mask);
    }
    else
    {
        void *default_format = rasterizer_get_bitmap_default_hardware_format(bitmap);
        D3DDevice_SetTexture(global_d3d_device, stage, (D3DBaseTexture *)default_format, pending_mask);
    }
    return 1;
}
