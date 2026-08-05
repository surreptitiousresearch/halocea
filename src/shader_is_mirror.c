/* shader_is_mirror @0x83755798 — type-specific mirror test on a shader tag. Both loads reach a member of the
 * *derived* tag, not of the 40-byte base: Hex-Rays folded byte offset 0x2D0 into shader[18] (= 18 * 40) and
 * byte offset 0x8A into shader[3] (= 3 * 40 + 0x10 + 2) because it only knew the base `shader` type. */

#include "headers/shader.h"
#include "headers/shader_type.h"
#include "headers/shader_environment.h"
#include "headers/shader_environment_reflection_mirror_flags.h"
#include "headers/shader_transparent_glass.h"
#include "headers/shader_transparent_glass_reflection_type.h"

BOOL shader_is_mirror(shader *shader)
{
    if (!shader)
        return 0;

    switch (shader->base.type)
    {
    case _shader_type_environment:
        /* DEVIATION: decompiler folded 0x2D0 into shader[18].base.radiosity.flags; lhz r11, 0x2D0(r11)
         * @0x837557D0 is shader_environment.environment.reflection.flags. */
        return ((const shader_environment *)shader)->environment.reflection.flags
            & (1u << _shader_environment_reflection_mirror_bit);
    case _shader_type_transparent_glass:
        /* DEVIATION: decompiler folded 0x8A into *((uint16_t *)&shader[3].base.radiosity.color.blue + 1);
         * lhz r11, 0x8A(r11) @0x837557BC is shader_transparent_glass.glass.reflection_type. */
        return ((const shader_transparent_glass *)shader)->glass.reflection_type
            == _shader_transparent_glass_reflection_type_mirror;
    default:
        return 0;
    }
}
