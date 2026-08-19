/* unit_set_controllable @0x836C9BD8 — toggle the "controllable" flag (bit 0x40) in unit.flags. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_set_controllable(int unit_index, uint8_t controllable)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unsigned int flags = unit->unit.flags | (1u << _unit_controllable_bit);
    if ( !controllable )
        flags = unit->unit.flags & ~(1u << _unit_controllable_bit);
    unit->unit.flags = flags;
}
