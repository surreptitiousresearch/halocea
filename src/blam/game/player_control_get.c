/* player_control_get @0x836DE258 */
#include <stdint.h>
#include "headers/player_control_globals.h"

player_control * player_control_get(int16_t local_player_index)
{
    return &player_control_globals->players[local_player_index];
}
