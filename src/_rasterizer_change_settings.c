#include <stdint.h>
#include "headers/rasterizer_settings_parameters.h"

int _rasterizer_change_settings(rasterizer_settings_parameters *parameters, uint8_t force_change)
{
    (void)parameters;
    (void)force_change;
    return 1;
}
