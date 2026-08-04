/* weapon_state_next @0x836DBCD0 — advance a weapon's animation state machine when the current animation
 * finishes. Most states fall back to idle; the charged states (7,8) and put-away (10) are left in place
 * (they persist until an explicit transition). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_state.h"
#include "headers/blam_data_globals.h"

extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);

void weapon_state_next(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    unsigned int state = (unsigned char)weapon->weapon.state;
    if ( state < 7 || (state > 8 && state != 10) )
        weapon_set_state(weapon_index, weapon_state_idle, 1);
}
