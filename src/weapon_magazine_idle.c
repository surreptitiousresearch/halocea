/* weapon_magazine_idle @0x836D9BE0 — force a magazine back to the idle state (clears state and
 * state_timer). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_magazine_state.h"
#include "headers/blam_data_globals.h"

void weapon_magazine_idle(int weapon_index, int16_t magazine_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_magazine *magazine = &weapon->weapon.magazines[magazine_index];

    magazine->state = _magazine_idle;
    magazine->state_timer = 0;
}
