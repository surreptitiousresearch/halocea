/* local_time_get @0x8369A898 */
#include "headers/game_time_globals.h"

int local_time_get(void)
{
    return game_time_globals->local_time;
}
