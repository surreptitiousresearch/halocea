/* unit_scripting_set_seat @0x836CFE48 — set a unit's base seat index (unit data +0x20F) from a seat label
 * string, resolved via seat_label_to_base_seat_index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int16_t seat_label_to_base_seat_index(const char *seat_label);

void unit_scripting_set_seat(int unit_index, const char *seat_label)
{
    if ( unit_index == -1 )
        return;
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit->unit.magic_seat_index = seat_label_to_base_seat_index(seat_label);
}
