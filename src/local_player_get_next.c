/* local_player_get_next @ 0x836A80DC — iterate local players in ascending index order. Pass
 * -1 to start; returns the next occupied local-player slot index, or -1 when exhausted. */

#include <stdint.h>
#include "headers/players_global_data.h"

int16_t local_player_get_next(int16_t local_player_index)
{
    int16_t best = -1;
    int16_t i;

    for ( i = 0; i < 2; ++i )
    {
        if ( players_globals->local_player_indices[i] != -1
          && i > local_player_index
          && (i < best || best == -1) )
        {
            best = i;
        }
    }
    return best;
}
