/* weapon_trigger_finish_tracking @0x836DC288 — end a weapon trigger's target-tracking phase: clear the
 * weapon's tracked object and reset the trigger's idle counter and firing state. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/weapon_trigger_state.h"

void weapon_trigger_finish_tracking(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    weapon->weapon.tracked_object_index = -1;

    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];
    trigger->idle_ticks = 0;
    trigger->state = _weapon_trigger_idle;
    trigger->state_timer = 0;
}
