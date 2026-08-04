#include <stdint.h>
#include "headers/netgame_goal.h"
#include "headers/blam_data_globals.h"


uint8_t game_engine_get_goal_in_use(int16_t index)
{
    return global_goal[index].in_use;
}
