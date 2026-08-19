/* unit_set_possessed @0x836C9C20 — toggle the "possessed" flag (bit 0x8000000) in unit.flags. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_set_possessed(int unit_index, uint8_t possessed)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unsigned int flags = unit->unit.flags | (1u << _unit_possessed_by_recording_bit);
    if ( !possessed )
        flags = unit->unit.flags & ~(1u << _unit_possessed_by_recording_bit);
    unit->unit.flags = flags;
}
