/* shader_is_water_decal @0x83755850 — true if a transparent shader (types 5/6/7) is flagged as a water decal.
 * The flag is bit 4 of the radiosity-properties flags of the *second* shader element (shader[1], i.e. the
 * byte at shader + 0x29). Non-transparent or null shaders are never water decals.
 *
 * DEVIATION: the original read the raw byte at shader+0x29 (low byte of the big-endian u16 radiosity.flags);
 * expressed here as the equivalent named-field bit test now that radiosity is a typed sub-struct. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"

uint8_t shader_is_water_decal(const shader *shader)
{
    if (!shader)
        return 0;

    unsigned __int16 type = shader->base.type;
    if (type == _shader_type_transparent_generic
        || type == _shader_type_transparent_chicago
        || type == _shader_type_transparent_chicago_extended)
        return (shader[1].base.radiosity.flags >> 4) & 1;  /* draw_before_water_bit (4) */

    return 0;
}
