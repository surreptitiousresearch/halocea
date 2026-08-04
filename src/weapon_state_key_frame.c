/* weapon_state_key_frame @0x836DA0C8 — animation key-frame callback for a weapon: on the chamber states
 * (primary/secondary chamber) it kicks off the corresponding trigger's ejection-port animation. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_state.h"
#include "headers/blam_data_globals.h"

extern void weapon_trigger_start_ejection_port(int weapon_index, int16_t trigger_index, uint8_t chamber);

void weapon_state_key_frame(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    unsigned char state = (unsigned char)weapon->weapon.state;
    if ( state == weapon_state_primary_chamber )
        weapon_trigger_start_ejection_port(weapon_index, 0, 1);
    else if ( state == weapon_state_secondary_chamber )
        weapon_trigger_start_ejection_port(weapon_index, 1, 1);
}
