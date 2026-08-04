#include "headers/rasterizer_settings_parameters.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/ppc_intrinsics.h"
extern int _cntlzw(unsigned int);

void _rasterizer_get_settings(rasterizer_settings_parameters *parameters)
{
    parameters->screen_width  = global_d3d_present_parameters.BackBufferWidth;
    parameters->screen_height = global_d3d_present_parameters.BackBufferHeight;
    parameters->refresh_rate  = global_d3d_present_parameters.FullScreen_RefreshRateInHz;
    parameters->vsync         = (_cntlzw(global_d3d_present_parameters.PresentationInterval - 1) & 0x20) != 0;
}
