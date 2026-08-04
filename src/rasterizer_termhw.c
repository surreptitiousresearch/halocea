/* rasterizer_termhw @0x8369B978 — tear down the rasterizer's hardware-dependent state before a device
 * reset/shutdown. Unbinds every sampler texture, clears the vertex and pixel shaders, then disposes
 * the dynamic vertex buffers, render targets, shaders, and occlusion-test query. No-op if the
 * rasterizer was never initialized.
 *
 * DEVIATIONS:
 *   - The per-stage SetTexture call binds a null texture with the per-stage pending mask
 *     1ULL << (31 - stage) (same one-hot form as rasterizer_set_texture_bitmap_data); the decompiler
 *     swapped the texture and mask arguments and left a dead HIDWORD store.
 *   - The dispose chain threads r3 through each callee; every dispose is void(void), so reproduced as
 *     a plain sequence. */

#include "headers/blam_data_globals.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/chicago_pixel_shader.h"  /* D3DPixelShader */

extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);
extern void rasterizer_dx9_dynamic_vertex_buffers_dispose(void);
extern D3DResource *rasterizer_render_targets_dispose(void);
extern void rasterizer_shaders_dispose(void);
extern D3DQuery *rasterizer_occulsion_test_dispose(void);

void rasterizer_termhw(void)
{
    if (!rasterizer_initialized)
        return;

    for (unsigned int stage = 0; stage < global_d3d_caps.MaxSimultaneousTextures; stage++)
        D3DDevice_SetTexture(global_d3d_device, stage, nullptr, 1ULL << (31 - stage));

    D3DDevice_SetVertexShader(global_d3d_device, nullptr);
    D3DDevice_SetPixelShader(global_d3d_device, nullptr);

    rasterizer_dx9_dynamic_vertex_buffers_dispose();
    rasterizer_render_targets_dispose();
    rasterizer_shaders_dispose();
    rasterizer_occulsion_test_dispose();
}
