/* player_profile_save_last_level_played @0x83765B68 — record the current solo (campaign) level as the
 * "last level played" in a local player's active profile, persisting it if it changed. */

#include <stdint.h>
#include "headers/player_profile.h"

extern int16_t main_get_current_solo_level(void);
extern int player_ui_get_active_player_profile_index(int16_t local_player_index);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);
extern int player_profile_write(int index, player_profile *profile);

void player_profile_save_last_level_played(int16_t local_player_index)
{
    int16_t current_solo_level = main_get_current_solo_level();
    if ( current_solo_level != -1 )
    {
        int active_player_profile_index = player_ui_get_active_player_profile_index(local_player_index);
        if ( active_player_profile_index != -1 )
        {
            player_profile profile;
            player_ui_get_active_player_profile(local_player_index, &profile);
            if ( profile.last_single_player_map_played != current_solo_level )
            {
                profile.last_single_player_map_played = current_solo_level;
                player_profile_write(active_player_profile_index, &profile);
            }
            player_ui_set_active_player_profile(local_player_index, active_player_profile_index, &profile);
        }
    }
}
