/* shader_is_mirror @0x83755798 — type-specific mirror-flag test: for an environment shader (type 3), tests
 * bit 0 of shader[18]'s radiosity flags; for a transparent_glass shader (type 9), tests the low 16 bits of
 * shader[3]'s radiosity color.blue against the mirror reflection type (2) (both are byte-offset
 * reinterpretations of the base shader array at a different element stride, per disasm).
 * The glass compare value 2 = _shader_transparent_glass_reflection_type_mirror
 * (DB enum $E160613C0679F6AD80E5248BEAFC0911). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"
#include "headers/shader_transparent_glass_reflection_type.h"

BOOL shader_is_mirror(shader *shader)
{
    if (!shader)
        return 0;

    switch (shader->base.type)
    {
    case _shader_type_environment:
        return shader[18].base.radiosity.flags & 1;
    case _shader_type_transparent_glass:
        return *((uint16_t *)&shader[3].base.radiosity.color.blue + 1)
            == _shader_transparent_glass_reflection_type_mirror;
    default:
        return 0;
    }
}
