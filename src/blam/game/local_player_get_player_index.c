/* local_player_get_player_index @ 0x836A8020 — the global player index controlled by a given local
 * player slot, or -1 if the slot is unused. */

#include <stdint.h>
#include "headers/players_global_data.h"

int local_player_get_player_index(int16_t local_player_index)
{
    if ( local_player_index == -1 )
        return -1;
    return players_globals->local_player_indices[local_player_index];
}
