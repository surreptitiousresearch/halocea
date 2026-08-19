/* game_engine_set_goal_position @0x83748350 — set up one multiplayer goal marker (global_goal[index]):
 * resolve `descriptor` to a nav point index, mark it in use, copy the given position, record the
 * player/team it targets and the player it should be ignored for, and lift the goal's z position by
 * vertical_offset plus a fixed 0.63 unit clearance.
 *
 * DEVIATION: this ABI reserves a shadow GPR slot for the float `vertical_offset` parameter (passed in f1),
 * which the decompiler failed to account for — it shifted every subsequent integer/pointer parameter down by
 * one register, mislabeling `descriptor` (r6) as `player_index`, `player_index` (r7) as `team_index`,
 * `team_index` (r8) as `ignore_player_index`, and fabricating a phantom trailing "a8" for the true
 * `ignore_player_index` (r9). Confirmed via disasm: `mr r3,r6` feeds find_nav_point (which takes a
 * `const char*`, matching `descriptor`, not an int), and the field stores land on the correct netgame_goal
 * members once each register is reattributed one slot later than the decompiler's own numbering. The
 * underlying stored bytes were always correct — only the decompiler's local-variable labels were wrong. */

#include <stdint.h>
#include "headers/netgame_goal.h"
#include "headers/blam_data_globals.h"


extern int find_nav_point(const char *name);

void game_engine_set_goal_position(int16_t index, real_point3d *position, float vertical_offset, char *descriptor, int player_index, int16_t team_index, int ignore_player_index)
{
    netgame_goal *goal = &global_goal[index];

    goal->ignore_player_index = ignore_player_index;
    goal->nav_index = find_nav_point(descriptor);
    goal->in_use = 1;
    goal->position = *position;
    goal->team_index = team_index;
    goal->player_index = player_index;
    goal->position.n[2] = (vertical_offset + goal->position.n[2]) + 0.63f;
}
