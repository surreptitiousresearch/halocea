/* unit_scripting_exit_vehicle @0x836D64A8 — if the unit is currently in a seat of a parent object
 * (parent +0x11C != -1 and seat index +0x2F0 != 0xFFFF), make it exit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_try_and_exit_seat(int object_index, uint8_t should_allow_clients);

void unit_scripting_exit_vehicle(int unit_index)
{
    if ( unit_index == -1 )
        return;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( unit->object.parent_object_index != -1
         && (uint16_t)unit->unit.parent_seat_index != 0xFFFF )
        unit_try_and_exit_seat(unit_index, 0);
}
