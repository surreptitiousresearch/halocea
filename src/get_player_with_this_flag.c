/* get_player_with_this_flag @0x83806608 — find the player whose current unit is holding the given weapon
 * (e.g. the flag/oddball carrier); returns that player's data-array index, or -1 if none. Player record
 * dword +13 (+52 bytes) is the controlled unit index (-1 = dead), matching any_player_is_dead.c. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_has_weapon(int unit_index, int weapon_index);

int get_player_with_this_flag(int weapon_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return -1;

    while ( 1 )
    {
        int unit_index = player->unit_index;
        if ( unit_index != -1 && unit_has_weapon(unit_index, weapon_index) )
            break;

        player = data_iterator_next(&iterator);
        if ( !player )
            return -1;
    }

    return iterator.index;
}
