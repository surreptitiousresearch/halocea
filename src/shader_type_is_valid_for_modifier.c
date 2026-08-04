/* shader_type_is_valid_for_modifier 0x83755978 — true if a shader of the given type can be used as a model
 * effect's modifier shader: effect (1), or the transparent_generic..transparent_plasma family (5..11). */

#include <stdint.h>
#include "headers/shader_type.h"

uint8_t shader_type_is_valid_for_modifier(int16_t shader_type)
{
    if ( shader_type == _shader_type_effect
        || (shader_type > _shader_type_model && shader_type <= _shader_type_transparent_plasma) )
        return 1;
    return 0;
}
