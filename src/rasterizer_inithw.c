/* rasterizer_inithw @0x8369DB60 — (re)initialize the rasterizer's hardware-dependent state after a
 * device reset. If the rasterizer was never initialized, returns success immediately. Otherwise it
 * restores the default render state and re-creates shaders, render targets, and the occlusion-test
 * query; failure of any of these yields a 0 result. Dynamic vertex buffers are always restored.
 *
 * DEVIATION: the decompiler threaded rasterizer_set_default_state()'s r3 into
 * rasterizer_shaders_initialize; the callees are void(void), so that is register-reuse noise. */

#include <stdint.h>
#include "headers/blam_data_globals.h"

extern void _rasterizer_set_default_state(void);
extern uint8_t rasterizer_shaders_initialize(void);
extern uint8_t rasterizer_render_targets_initialize(void);
extern uint8_t rasterizer_occulsion_test_initialize(void);
extern void rasterizer_dx9_dynamic_vertex_buffers_restore(void);

int rasterizer_inithw(void)
{
    if (!rasterizer_initialized)
        return 1;

    _rasterizer_set_default_state();

    int result = 1;
    if (!rasterizer_shaders_initialize()
        || !rasterizer_render_targets_initialize()
        || !rasterizer_occulsion_test_initialize())
    {
        result = 0;
    }

    rasterizer_dx9_dynamic_vertex_buffers_restore();
    return result;
}
