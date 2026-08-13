/* weapon_trigger_idle @0x836DB430 — return a weapon trigger to the idle firing state (clears state and
 * state_timer). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/weapon_trigger_state.h"

void weapon_trigger_idle(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    trigger->state = _weapon_trigger_idle;
    trigger->state_timer = 0;
}
