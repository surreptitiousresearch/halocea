/* rasterizer_change_settings @0x836A5CB8 */
#include <stdint.h>
#include "headers/rasterizer_settings_parameters.h"

extern int _rasterizer_change_settings(rasterizer_settings_parameters *parameters, uint8_t force_change);

int rasterizer_change_settings(rasterizer_settings_parameters *parameters, uint8_t force_change)
{
    return _rasterizer_change_settings(parameters, force_change);
}
