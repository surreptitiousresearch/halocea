/* shader_ignores_effect @0x83755888 — for the 3 shader types that support an "ignore effect" toggle (types
 * 5/6/7), reads bit 5 of the immediately-following variant-specific shader block's radiosity.flags (i.e.
 * `shader[1]`, the block right after this shader's own generic header — the type-specific extension struct
 * reinterpreted as another `shader`, same idiom used elsewhere for shader variant overlays). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"

uint8_t shader_ignores_effect(const shader *shader)
{
    if ( !shader )
        return 0;
    int type = (unsigned __int16)shader->base.type;
    if ( type == _shader_type_transparent_generic
        || type == _shader_type_transparent_chicago
        || type == _shader_type_transparent_chicago_extended )
        return (shader[1].base.radiosity.flags >> 5) & 1;  /* ignore_effect_bit (5) */
    return 0;
}
