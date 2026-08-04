/* local_player_exists @0x836A7B80 — true if a player record exists for the given local-player index
 * (matched on player_datum.local_player_index). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

/* attested uint8_t: both sampled callers clrlwi r3,24 (byte-normalize) */
uint8_t local_player_exists(int local_player_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return 0;
    while ( player->local_player_index != local_player_index )
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return 0;
    }
    return 1;
}
