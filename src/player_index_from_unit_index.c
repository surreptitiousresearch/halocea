/* player_index_from_unit_index @0x836A8140 — find the player whose controlled unit is unit_index,
 * returning that player's datum index, or -1. Player datum word 13 (+0x34) holds the unit index. */

#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


int player_index_from_unit_index(int unit_index)
{
    int index = -1;
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        if ( player->unit_index == unit_index )
            index = iterator.index;
    }
    return index;
}
