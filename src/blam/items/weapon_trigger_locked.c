/* weapon_trigger_locked @0x836DB478 — put a weapon trigger into the locked state (state 7) with an
 * indefinite (-1) state timer. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_trigger_state.h"
#include "headers/blam_data_globals.h"

void weapon_trigger_locked(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    trigger->state = _trigger_locked;
    trigger->state_timer = -1;   /* indefinite */
}
