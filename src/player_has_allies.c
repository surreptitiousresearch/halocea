/* player_has_allies @0x837C3298 — true if any OTHER active player shares this player's team. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"


uint8_t player_has_allies(int player_index)
{
    int team_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->team_index;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return 0;

    while ( iterator.index == player_index || player->team_index != team_index )
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return 0;
    }

    return 1;
}
