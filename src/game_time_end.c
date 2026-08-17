/* game_time_end @0x8369A850 */
#include "headers/game_time_globals.h"

void game_time_end(void)
{
    game_time_globals->active = 0;
}
