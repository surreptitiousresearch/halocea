/* director_inhibited_input @0x836E4548 — return whether camera-director input is inhibited for a local
 * player. */

#include <stdint.h>
#include "headers/director.h"

uint8_t director_inhibited_input(int16_t local_player_index)
{
    return director_globals.local_players[local_player_index].inhibited_input;
}
