/* unit_adjust_for_seat_change @0x836D2968 — after a unit enters/exits a seat, pick the weapon that should now
 * be desired (the next available weapon from the current one) and start readying it. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int16_t unit_weapon_next_index(int unit_index, int16_t current_index, int16_t delta);
extern void unit_ready_desired_weapon(int unit_index, uint8_t immediate);

void unit_adjust_for_seat_change(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit->unit.desired_weapon_index = unit_weapon_next_index(unit_index, unit->unit.current_weapon_index, 0);
    unit_ready_desired_weapon(unit_index, 1u);
}
