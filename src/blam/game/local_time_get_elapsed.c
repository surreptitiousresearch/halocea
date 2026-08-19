/* local_time_get_elapsed @0x8369A8A8 */
#include "headers/game_time_globals.h"

int local_time_get_elapsed(void)
{
    return (unsigned short)game_time_globals->last_local_time_elapsed;
}
