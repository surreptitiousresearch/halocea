/* shader_type_is_lightmapped @0x837558E8 — true for lightmapped shader types: environment (3), model (4),
 * or transparent_glass (9). */

#include <stdint.h>
#include "headers/shader_type.h"

uint8_t shader_type_is_lightmapped(int16_t shader_type)
{
    return shader_type >= _shader_type_environment
        && (shader_type <= _shader_type_model || shader_type == _shader_type_transparent_glass);
}
