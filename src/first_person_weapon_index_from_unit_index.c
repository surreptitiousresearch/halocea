/* first_person_weapon_index_from_unit_index @0x8369EA78 — find the local-player slot whose controlled
 * unit (player record dword +52) is the given unit, i.e. the unit's first-person weapon index. Returns
 * -1 if no local player controls the unit. Player records are stride 512. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int local_player_get_player_index(int16_t local_player_index);

int16_t first_person_weapon_index_from_unit_index(int unit_index)
{
    short local_player_index = 0;
    while ( 1 )
    {
        int player_index = local_player_get_player_index(local_player_index);
        if ( player_index != -1
          && DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index == unit_index )
            break;
        local_player_index = (short)(local_player_index + 1);
        if ( local_player_index >= 2 )
            return -1;
    }
    return local_player_index;
}
