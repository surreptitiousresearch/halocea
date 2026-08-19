/* weapon_put_away @0x836DCD80 — lower/holster a weapon. Unless `immediate`, it refuses (returns 0) while
 * the weapon is mid-action (either trigger state, either magazine state, or the weapon state nonzero).
 * It then enters weapon state 10 (put-away), runs the hcex "put_away" object state, clears the control
 * flags, resets the weapon, deletes any active overheated effect, and sends the first-person "put away"
 * message (11). Returns 1 on success, 0 if blocked. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_state.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);
extern void hcex_obj_set_state(int object_index, const char *state);
extern void weapon_reset(int weapon_index);
extern void effect_delete(int effect_index);
extern void first_person_weapon_message_from_weapon(int weapon_index, int16_t message_type);

uint8_t weapon_put_away(int weapon_index, uint8_t immediate)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    if ( !immediate )
    {
        if ( weapon->weapon.triggers[0].state
          || weapon->weapon.triggers[1].state
          || weapon->weapon.magazines[0].state
          || weapon->weapon.magazines[1].state
          || weapon->weapon.state )
        {
            return 0;
        }
    }

    if ( !weapon_set_state(weapon_index, weapon_state_put_away, immediate) )
        return 0;

    hcex_obj_set_state(weapon_index, "put_away");
    weapon->weapon.control_flags = 0;
    weapon_reset(weapon_index);

    int effect_index = weapon->weapon.overheated_effect_index;
    if ( effect_index != -1 )
    {
        effect_delete(effect_index);
        weapon->weapon.overheated_effect_index = -1;
    }

    first_person_weapon_message_from_weapon(weapon_index, _first_person_weapon_message_put_away);
    return 1;
}
