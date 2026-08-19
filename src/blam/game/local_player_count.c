/* local_player_count @ 0x836A80C8 */
#include <stdint.h>
#include "headers/players_global_data.h"
int16_t local_player_count(void)
{
    return players_globals->local_player_count;
}
