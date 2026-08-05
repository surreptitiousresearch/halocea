#include <stdint.h>
#include "headers/rasterizer_settings_parameters.h"

extern int _rasterizer_needs_change(rasterizer_settings_parameters *parameters);

int rasterizer_needs_change(rasterizer_settings_parameters *parameters)
{
    return _rasterizer_needs_change(parameters);
}
