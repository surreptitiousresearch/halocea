/* _rasterizer_models_begin @0x8378AD18 — begin a model render batch: when model drawing is enabled, mark
 * the pixel shader dirty and record whether this batch is the sky. */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/blam_data_globals.h"


void _rasterizer_models_begin(uint8_t sky)
{
    if (rasterizer_debug_options.draw_models)
    {
        local_pixel_shader_dirty_flag = 1;
        local_sky_flag = sky;
    }
}
