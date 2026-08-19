/* player_control_get_autoaim_level @0x836DEA00 — return a local player's current auto-aim magnetism level.
 *
 * DEVIATION: the decompiler routed the float return through a double (soft-float ABI artifact). */

#include <stdint.h>
#include "headers/player_control_globals.h"

float player_control_get_autoaim_level(int16_t local_player_index)
{
    return player_control_globals->players[local_player_index].autoaim_level;
}
