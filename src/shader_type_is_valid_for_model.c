/* shader_type_is_valid_for_model @0x83755958 — true for environment..transparent_plasma (3..11). */

#include <stdint.h>
#include "headers/shader_type.h"

uint8_t shader_type_is_valid_for_model(int16_t shader_type)
{
    return shader_type >= _shader_type_environment && shader_type <= _shader_type_transparent_plasma;
}
