/* _rasterizer_frame_end @0x8369C278 — platform rasterizer frame-end implementation
 * (called through the rasterizer_frame_end thunk). Releases the GPU bindings held
 * during the frame (texture samplers, vertex stream, index buffer) and finalizes
 * frame statistics. */

#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_frame_statistics_end(void);

void _rasterizer_frame_end(void)
{
    int sampler;
    int stream;

    /* Unbind both fragment texture samplers. PendingMask3 selects this sampler's
     * GPU pending register (top bit shifted down by the sampler index). */
    for (sampler = 0; sampler < 2; sampler++)
        D3DDevice_SetTexture(global_d3d_device, sampler, 0,
                             0x8000000000000000ull >> (sampler + 32));

    /* Unbind the single active vertex stream. The (95-stream)*0x5556>>16 idiom is the
     * compiler computing the fetch-constant register number for this stream. */
    for (stream = 0; stream < 1; stream++)
        D3DDevice_SetStreamSource(global_d3d_device, stream, 0, 0, 0,
                                  0x8000000000000000ull >> ((((95 - stream) * 0x5556) >> 16) + 32));

    D3DDevice_SetIndices(global_d3d_device, 0);

    if (rasterizer_debug_options.statistics_mode)
        rasterizer_frame_statistics_end();
}
