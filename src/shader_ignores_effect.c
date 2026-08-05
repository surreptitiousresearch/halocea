/* shader_ignores_effect @0x83755888 — for the 3 shader types that support an "ignore effect" toggle
 * (transparent_generic / transparent_chicago / transparent_chicago_extended), reads that tag's own
 * "ignore effect" flag bit.
 *
 * DEVIATION: the decompiler folded the read into `shader[1].base.radiosity.flags` on the 40-byte base.
 * The real load is `lbz r11, 0x29(r11)` / `extrwi r3, r11, 1,26` @0x837558B4 — a BYTE at +0x01 of the
 * derived tag body (which starts at +0x28), i.e. the uint8_t `flags` that follows numeric_counter_limit,
 * bit 5. All three tag types share a byte-identical {numeric_counter_limit, flags, type} prefix, so the
 * compiler emitted a single load for the three cases; typed here through shader_transparent_generic. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"
#include "headers/shader_transparent_generic.h"
#include "headers/shader_transparent_generic_flags.h"

uint8_t shader_ignores_effect(const shader *shader)
{
    if ( !shader )
        return 0;
    int type = (uint16_t)shader->base.type;
    if ( type == _shader_type_transparent_generic
        || type == _shader_type_transparent_chicago
        || type == _shader_type_transparent_chicago_extended )
        return (((const shader_transparent_generic *)shader)->generic.flags
                >> _shader_transparent_generic_ignore_effect_bit) & 1;
    return 0;
}
