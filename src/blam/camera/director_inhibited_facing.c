/* director_inhibited_facing @0x836E4528 — return whether camera-director facing control is inhibited for a
 * local player. */

#include <stdint.h>
#include "headers/director.h"

uint8_t director_inhibited_facing(int16_t local_player_index)
{
    return director_globals.local_players[local_player_index].inhibited_facing;
}
