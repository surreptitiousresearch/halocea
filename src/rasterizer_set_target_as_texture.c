/* rasterizer_set_target_as_texture @0x836F3810 — bind a render target's texture to a fixed-function
 * sampler stage, returning the bound texture (null if the target index is out of range). The pending
 * mask is the per-stage one-hot 1ULL << (31 - stage), matching rasterizer_set_texture_bitmap_data.
 *
 * DEVIATIONS:
 *   - The decompiler emitted "local variable allocation has failed" and scrambled the SetTexture
 *     texture/mask arguments; the disassembly (lwzx r31 from global_render_targets[target].texture,
 *     extldi/srd building 1<<63 >> (stage+32)) gives the clean form below.
 *   - The declared max_mipmap / trailing parameters are never referenced by the code (only r3=stage
 *     and r4=target are read); omitted from the signature. */

#include <stdint.h>
#include "headers/rasterizer_render_target.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"

D3DTexture * rasterizer_set_target_as_texture(int16_t stage, int16_t target)
{
    D3DTexture *texture = nullptr;

    if ((unsigned int)target <= 9)
        texture = global_render_targets[target].texture;

    D3DDevice_SetTexture(global_d3d_device, stage, (D3DBaseTexture *)texture, 1ULL << (31 - stage));
    return texture;
}
