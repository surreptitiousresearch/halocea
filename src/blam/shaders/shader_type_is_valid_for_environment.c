/* shader_type_is_valid_for_environment @0x83755930 — true for environment (3), or the
 * transparent_generic..transparent_meter family (5..10). */

#include <stdint.h>
#include "headers/shader_type.h"

uint8_t shader_type_is_valid_for_environment(int16_t shader_type)
{
    return shader_type == _shader_type_environment
        || (shader_type > _shader_type_model && shader_type <= _shader_type_transparent_meter);
}
