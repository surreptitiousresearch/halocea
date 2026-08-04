/* player_ui_get_active_player_profile_index @0x83698A60 — return the stored profile index for a local player
 * (_player_data.profile_index @1980), or -1 for an invalid player index. */

#include <stdint.h>
#include "headers/player_ui_globals.h"

int player_ui_get_active_player_profile_index(int16_t local_player_index)
{
    if ( (unsigned int)local_player_index > 1 )
        return -1;
    return player_ui_globals.player_data[local_player_index].profile_index;
}
