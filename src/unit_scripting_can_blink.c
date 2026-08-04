/* unit_scripting_can_blink @0x836C9DB8 — enable or disable a unit's ability to blink by toggling the
 * "cannot blink" flag (unit data +0x204 bit 0x400000): set when can_blink is false, cleared when true. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_scripting_can_blink(int unit_index, uint8_t can_blink)
{
    if ( unit_index == -1 )
        return;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unsigned int flags = unit->unit.flags | (1u << _unit_cannot_blink_bit);
    if ( can_blink )
        flags = unit->unit.flags & ~(1u << _unit_cannot_blink_bit);
    unit->unit.flags = flags;
}
