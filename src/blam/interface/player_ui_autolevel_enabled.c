/* player_ui_autolevel_enabled @0x83698B90 — whether auto-level (aim assist leveling) is enabled for the
 * player on a controller. In a network game the controller index is used directly; otherwise it is
 * mapped to a single-player local-player slot. Returns the per-player autolevel flag.
 *
 * DEVIATION: the decompiler indexes join_players_to_next_multiplayer_game with (1988*idx - 3672); that
 * resolves to player_data[idx].profile.controller_settings.autocenter (@304). */

#include <stdint.h>
#include "headers/player_ui_globals.h"

extern uint8_t network_game_is_active(void);
extern int16_t player_ui_get_single_player_local_player_from_controller(int16_t controller_index);

uint8_t player_ui_autolevel_enabled(int16_t controller_index)
{
    int16_t local_player_index = controller_index;
    if ( !network_game_is_active() )
        local_player_index = player_ui_get_single_player_local_player_from_controller(controller_index);
    if ( local_player_index == -1 )
        return 0;
    return player_ui_globals.player_data[local_player_index].profile.controller_settings.autocenter;
}
