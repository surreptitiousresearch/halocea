/* observer_get_camera @ 0x8370EAC0 — the resolved camera (observer_result) for a local player, or
 * null when there is no such local player (index -1). */

#include <stdint.h>
#include "headers/observer_globals.h"

const observer_result *observer_get_camera(int16_t local_player_index)
{
    if ( local_player_index != -1 )
        return &observer_globals.local_players[local_player_index].result;
    return 0;
}
