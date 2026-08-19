/* weapon_trigger_change_state @0x836D9ED0 — set a weapon trigger's firing state and its state timer. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"

void weapon_trigger_change_state(int weapon_index, int16_t trigger_index, int16_t new_state, int16_t new_state_timer)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    trigger->state = (char)new_state;
    trigger->state_timer = new_state_timer;
}
