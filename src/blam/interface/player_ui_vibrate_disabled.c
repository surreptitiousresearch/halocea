/* player_ui_vibrate_disabled @0x83698B60 — whether controller vibration is disabled in a local player's UI
 * profile. The flag lives at player_data[idx].profile.controller_settings.vibration_disabled (@302).
 * DEVIATION: the decompiler anchored this on join_players_to_next_multiplayer_game with a negative index
 * (1988*idx - 3674); that resolves to the vibration_disabled field. */

#include <stdint.h>
#include "headers/player_ui_globals.h"

uint8_t player_ui_vibrate_disabled(int16_t local_player_index)
{
    if ( local_player_index == -1 )
        return 0;
    return player_ui_globals.player_data[local_player_index].profile.controller_settings.vibration_disabled;
}
