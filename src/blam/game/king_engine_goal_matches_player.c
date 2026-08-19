/* king_engine_goal_matches_player @0x8382D038 — true when a player is NOT currently on the hill.
 * `goal_index` is unused (confirmed via disasm: only r3/player_index is read). */

#include <stdint.h>
#include "headers/king_globals.h"

uint8_t king_engine_goal_matches_player(int player_index, int goal_index)
{
    return king_globals.on_the_hill[player_index] == 0;
}
