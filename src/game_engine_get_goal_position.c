/* game_engine_get_goal_position @0x83748320 — copy the world position of the indexed multiplayer goal into the
 * caller-provided point and return it. */

#include <stdint.h>
#include "headers/netgame_goal.h"
#include "headers/blam_data_globals.h"


real_point3d *game_engine_get_goal_position(real_point3d *result, int16_t index)
{
    netgame_goal *goal = &global_goal[index];
    result->n[0] = goal->position.n[0];
    result->n[1] = goal->position.n[1];
    result->n[2] = goal->position.n[2];
    return result;
}
