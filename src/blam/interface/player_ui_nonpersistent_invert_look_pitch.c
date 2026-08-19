/* player_ui_nonpersistent_invert_look_pitch @0x83699AF0 */
#include <stdint.h>
#include "headers/player_ui_globals.h"

extern void set_local_player_controls_from_player_profile(int16_t local_player_index);

/* Toggle a guest (no bound profile) local player's invert-look-pitch setting
 * without persisting it, and return the resulting state.  DEVIATION: decompiler
 * rendered the field accesses as raw 1988-byte-stride indexing off
 * join_players_to_next_multiplayer_game; offsets resolve to
 * player_data[idx].profile_index (0x7BC) and
 * player_data[idx].profile.controller_settings.invert_look (0x12D). The
 * `(_cntlzw(x) & 0x20) != 0` idiom is a logical NOT (32 leading zeros iff x==0). */
int player_ui_nonpersistent_invert_look_pitch(int16_t local_player_index)
{
    if ( local_player_index != -1
      && player_ui_globals.player_data[local_player_index].profile_index == -1 )
    {
        player_ui_globals.player_data[local_player_index].profile.controller_settings.invert_look =
            !player_ui_globals.player_data[local_player_index].profile.controller_settings.invert_look;
        set_local_player_controls_from_player_profile(local_player_index);
    }

    if ( local_player_index != -1 )
        return player_ui_globals.player_data[local_player_index].profile.controller_settings.invert_look;
    return 0;
}
