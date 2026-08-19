/* game_engine_clear_goal_position @0x837483F8 — zero out the goal entry at `index`.
 *
 * DEVIATION: the decompiler renders this as an 8-dword loop starting from `&global_goal[index-1].nav_index`
 * and pre-incrementing before each store; disasm (slwi r9,r11,5 = index*32, then addi r11,r11,-4 with a
 * stwu-based loop) shows the writes actually land at offsets [0,32) of global_goal[index] directly (0-based,
 * matching sizeof(netgame_goal) exactly) — reproduced as the equivalent memset. */

#include <stdint.h>
#include <string.h>
#include "headers/netgame_goal.h"
#include "headers/blam_data_globals.h"


void game_engine_clear_goal_position(int16_t index)
{
    memset(&global_goal[index], 0, sizeof(netgame_goal));
}
