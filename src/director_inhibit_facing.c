/* director_inhibit_facing @0x836E44E8 */
#include <stdint.h>
#include "headers/director.h"

void director_inhibit_facing(int16_t local_player_index)
{
    director_globals.local_players[local_player_index].inhibited_facing = 1;
}
