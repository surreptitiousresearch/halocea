#include <stdint.h>
#include "headers/global_window_parameters.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);

void _rasterizer_environment_shadow_end(void)
{
    if (global_window_parameters.rasterizer_target == _rasterizer_target_render_primary
        && !rasterizer_globals.render_targets_disabled
        && rasterizer_debug_options.draw_environment_shadows
        && !shadow_restored)
    {
        rasterizer_set_target(1, 0, 0, 0, 1);
        shadow_restored = 1;
    }
}
