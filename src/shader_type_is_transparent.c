/* shader_type_is_transparent @0x837558C0 — transparent shader types are effect (1) and the
 * transparent_generic..transparent_plasma family (5..11). */

#include <stdint.h>
#include "headers/shader_type.h"

uint8_t shader_type_is_transparent(int16_t shader_type)
{
    return shader_type == _shader_type_effect
        || (shader_type > _shader_type_model && shader_type <= _shader_type_transparent_plasma);
}
