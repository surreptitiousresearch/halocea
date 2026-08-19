/* weapon_trigger_recover @0x836DB4C8 — reset a weapon trigger's firing state back to idle: clears the
 * trigger's idle_ticks, state, and state_timer (weapon_datum.weapon.triggers[trigger_index]). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_trigger_state.h"
#include "headers/blam_data_globals.h"


void weapon_trigger_recover(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    trigger->idle_ticks = 0;
    trigger->state = _weapon_trigger_idle;
    trigger->state_timer = 0;
}
