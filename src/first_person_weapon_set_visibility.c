/* first_person_weapon_set_visibility @0x8369E7C0 — show or hide a local player's first-person weapon and
 * start/stop its attached effects and particles to match. A no-op when the requested visibility already
 * matches the current state. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/blam_data_globals.h"


extern void effects_start_on_first_person_weapon(int16_t local_player_index, int object_index);
extern void effects_stop_on_first_person_weapon(int16_t local_player_index);
extern void particles_stop_on_first_person_weapon(int16_t local_player_index);

void first_person_weapon_set_visibility(int16_t local_player_index, uint8_t visible)
{
    first_person_weapon *fp_weapon = &first_person_weapons[local_player_index];
    if ( visible != fp_weapon->visible )
    {
        if ( visible )
        {
            effects_start_on_first_person_weapon(local_player_index, fp_weapon->weapon_index);
        }
        else
        {
            effects_stop_on_first_person_weapon(local_player_index);
            particles_stop_on_first_person_weapon(local_player_index);
        }
        fp_weapon->visible = visible;
    }
}
