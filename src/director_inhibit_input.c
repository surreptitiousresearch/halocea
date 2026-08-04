/* director_inhibit_input @0x836E4508 — mark a local player's camera director as ignoring input this frame. */

#include <stdint.h>
#include "headers/director.h"

void director_inhibit_input(int16_t local_player_index)
{
    director_globals.local_players[local_player_index].inhibited_input = 1;
}
