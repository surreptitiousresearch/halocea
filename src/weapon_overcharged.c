/* weapon_overcharged @0x836D95C0 — true when a weapon object is in an overcharged/charging-overflow state
 * (its primary trigger state, weapon.triggers[0].state, is charging or charged). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_trigger_state.h"
#include "headers/blam_data_globals.h"


int weapon_overcharged(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    char charge_state = weapon->weapon.triggers[0].state;
    if (charge_state == _weapon_trigger_charging)
        return 1;
    if (charge_state == _weapon_trigger_charged)
        return 1;
    return 0;
}
