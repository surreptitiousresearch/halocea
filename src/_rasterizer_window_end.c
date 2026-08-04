/* _rasterizer_window_end @0x8369C198 — platform rasterizer window-end implementation
 * (called through the rasterizer_window_end thunk). Ends each per-window rasterizer
 * subsystem in order, then re-opens the debug pass for the next window. */

#include <stdint.h>
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_frame_statistics_s.h"
#include "headers/blam_data_globals.h"

#include "headers/rasterizer_frame_statistics_s.h"
extern void rasterizer_frame_statistics_get_fps(rasterizer_frame_statistics_s *frame_statistics, uint8_t frame_dropped);
extern void rasterizer_frame_statistics_draw(void);

extern void rasterizer_memory_pool_end(void);
extern void rasterizer_dynamic_geometry_end(void);
extern void rasterizer_transparent_geometry_end(void);
extern void rasterizer_lights_end(void);
extern void rasterizer_debug_end(void);
extern void rasterizer_debug_begin(void);

void _rasterizer_window_end(void)
{
    /* Only the final (top-level) window draws the FPS overlay. */
    if ((uint16_t)global_window_parameters.window_index == 0xFFFF)
    {
        rasterizer_frame_statistics_get_fps(&rasterizer_frame_statistics, 0);
        rasterizer_frame_statistics_draw();
    }

    /* Threaded-r3 artifact in the decompiler: these are independent no-arg calls
     * (no register setup between the bl instructions), not a nested chain. */
    rasterizer_memory_pool_end();
    rasterizer_dynamic_geometry_end();
    rasterizer_transparent_geometry_end();
    rasterizer_lights_end();
    rasterizer_debug_end();
    rasterizer_debug_begin();
}
