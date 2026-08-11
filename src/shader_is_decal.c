/* shader_is_decal @0x837557F0 0x837557F0 — true when the shader is a decal-style transparent shader that should be
 * routed through the decal pass. Only the transparent shader types (transparent_generic..transparent_meter,
 * 5..10) qualify; the answer is the concrete tag's own "decal" flag, and never set for transparent_water.
 *
 * DEVIATION: the decompiler folded every read into `shader[1].base.radiosity.flags` (byte 0x28) on the
 * 40-byte base, which hid that the binary issues THREE different loads — one per flags-field width:
 *   `lbz r11, 0x29` @0x8375582C, bit 1 — generic/chicago/chicago_extended, whose flags is a uint8_t at
 *        +0x01 of the derived body (+0x00 is numeric_counter_limit)
 *   `lhz r11, 0x28` @0x83755838, bit 1 — glass, whose flags is a uint16_t at +0x00
 *   `lhz r11, 0x28` @0x83755844, bit 0 — meter, whose flags is a uint16_t at +0x00 and whose decal bit
 *        is bit 0, not bit 1 */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"
#include "headers/shader_transparent_generic.h"
#include "headers/shader_transparent_generic_flags.h"
#include "headers/shader_transparent_chicago.h"
#include "headers/shader_transparent_chicago_extended.h"
#include "headers/shader_transparent_chicago_flags.h"
#include "headers/shader_transparent_glass.h"
#include "headers/shader_transparent_glass_flags.h"
#include "headers/shader_transparent_meter.h"
#include "headers/shader_transparent_meter_flags.h"

uint8_t shader_is_decal(const shader *shader)
{
    if ( shader && (unsigned int)(uint16_t)(shader->base.type - _shader_type_transparent_generic) <= 5 )
    {
        switch ( shader->base.type )
        {
            case _shader_type_transparent_generic:
                return (((const shader_transparent_generic *)shader)->generic.flags
                        >> _shader_transparent_generic_decal_bit) & 1;
            case _shader_type_transparent_chicago:
                return (((const shader_transparent_chicago *)shader)->chicago.flags
                        >> _shader_transparent_chicago_decal_bit) & 1;
            case _shader_type_transparent_chicago_extended:
                return (((const shader_transparent_chicago_extended *)shader)->chicago_extended.flags
                        >> _shader_transparent_chicago_decal_bit) & 1;
            case _shader_type_transparent_water:
                return 0;
            case _shader_type_transparent_glass:
                return (((const shader_transparent_glass *)shader)->glass.flags
                        >> _shader_transparent_glass_decal_bit) & 1;
            default:  /* _shader_type_transparent_meter */
                return (((const shader_transparent_meter *)shader)->meter.flags
                        >> _shader_transparent_meter_decal_bit) & 1;
        }
    }
    return 0;
}
