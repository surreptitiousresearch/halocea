/* king_player_changed_team @0x8382D2A0 — clears the player's King-of-the-Hill stat slot at word offset 98
 * (statistics.multiplayer_statistics.king_statistics.time_on_hill = king_statistics.time_on_hill, byte +196) when they change teams. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

void king_player_changed_team(int player_index, uint8_t new_team)
{
    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( player )
        player->statistics.multiplayer_statistics.king_statistics.time_on_hill = 0;  /* king_statistics.time_on_hill */
}
