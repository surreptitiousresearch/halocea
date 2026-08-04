#include <stdint.h>
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_target.h"

/* uint8_t return: sole caller byte-normalizes (clrlwi r11, r3, 24 @ 0x83812E68) — Blam boolean. */
uint8_t rasterizer_active_camouflage_require_zbuffer_fill(void)
{
    if (global_window_parameters.rasterizer_target != _rasterizer_target_render_primary || !rasterizer_debug_options.active_camouflage_enabled)
        return 0;
    if (rasterizer_globals.use_cheap_active_camo)
        return 1;
    if (rasterizer_globals.alpha_render_targets_disabled)
        return 1;
    return 0;
}
