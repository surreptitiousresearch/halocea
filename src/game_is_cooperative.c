#include <stdint.h>
#include "headers/blam_data_globals.h"
/* game_is_cooperative @0x83688600 — return whether the game is in cooperative mode, derived from the player
 * spawn count.
 *
 * DEVIATION: reproduced as the compiler's branchless range test on player_spawn_count. */


uint8_t game_is_cooperative(void)
{
    return ((player_spawn_count >= 0) + ((unsigned int)player_spawn_count <= 1)) & 1;
}
