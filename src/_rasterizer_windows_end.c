/* _rasterizer_windows_end @ 0x8369DA20
   Ends a render window: restores the default render target (unless alpha
   render targets are disabled) and marks the scene closed. */

#include <stdint.h>
#include "headers/rasterizer_globals.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);

void _rasterizer_windows_end(void)
{
    if ( rasterizer_globals.active == 1 && !rasterizer_globals.alpha_render_targets_disabled )
        rasterizer_set_target(0, 0, 0, 0, 0);
    InsideScene = 0;
}
