#include "headers/rasterizer_settings_parameters.h"

extern int _rasterizer_needs_change(rasterizer_settings_parameters *parameters);

unsigned __int8 rasterizer_needs_change(rasterizer_settings_parameters *parameters)
{
    return _rasterizer_needs_change(parameters);
}
