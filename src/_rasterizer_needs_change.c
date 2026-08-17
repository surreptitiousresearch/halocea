/* _rasterizer_needs_change @0x8369B910 */
#include "headers/rasterizer_settings_parameters.h"
#include "headers/rasterizer_engine_globals.h"

int _rasterizer_needs_change(rasterizer_settings_parameters *parameters)
{
    int result = global_d3d_present_parameters.BackBufferHeight != parameters->screen_height
              || global_d3d_present_parameters.BackBufferWidth != parameters->screen_width;
    if (use_fullscreen)
        return ((global_d3d_present_parameters.PresentationInterval == 1) ^ parameters->vsync) | result;
    return result;
}
