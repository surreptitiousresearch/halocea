/* cheat_player_index @0x83699F10 — return the player-table index of the first player that currently has a
 * living unit (player datum +0x34 unit index != -1), or -1 if none. Used as the "acting player" for the
 * debug cheat commands. */

#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


int cheat_player_index(void)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        if ( player->unit_index != -1 )
            return iterator.index;
    }
    return -1;
}
