/* first_person_weapons_update @ 0x836A0B98 — keep each local player's first-person viewmodel in sync.
 * For every active local player it re-reads the player's current unit; on a unit change it forces a
 * weapon re-switch and marks the viewmodel un-rendered. If no weapon is selected it switches weapons,
 * then ticks the viewmodel animation/pose state. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern void first_person_weapon_switch_weapons(int16_t local_player_index);
extern void first_person_weapon_update(int16_t local_player_index);

void first_person_weapons_update(void)
{
    int local_player;

    for ( local_player = 0; local_player < 2; local_player++ )
    {
        int player_index = local_player_get_player_index((int16_t)local_player);
        first_person_weapon *fpw;
        int unit_index;

        if ( player_index == -1 )
            continue;

        fpw = &first_person_weapons[local_player];
        unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
        if ( fpw->unit_index != unit_index )
        {
            fpw->unit_index = unit_index;
            fpw->rendered = 0;
            first_person_weapon_switch_weapons((int16_t)local_player);
        }
        if ( fpw->weapon_index == -1 )
            first_person_weapon_switch_weapons((int16_t)local_player);
        first_person_weapon_update((int16_t)local_player);
    }
}
