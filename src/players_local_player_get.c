/* players_local_player_get @0x836A8BE8 — the player_data datum index of the first local player (the first player
 * whose local-player index, datum +0x2, is not 0xFFFF). Returns -1 if there is no local player. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

int players_local_player_get(void)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return -1;
    while ( (uint16_t)player->local_player_index == 0xFFFF )  /* sentinel on signed __int16 */
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return -1;
    }
    return iterator.index;
}
