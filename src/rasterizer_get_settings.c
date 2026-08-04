#include "headers/rasterizer_settings_parameters.h"

extern void _rasterizer_get_settings(rasterizer_settings_parameters *parameters);

void rasterizer_get_settings(rasterizer_settings_parameters *parameters)
{
    _rasterizer_get_settings(parameters);
}
