/* player_profile_save_level_completed @0x83765BE8 — record, in the local player's active profile, that the
 * current solo level was completed at the current difficulty: set the difficulty bit in that level's
 * single_player_map_flags, write the profile to disk, and re-install it as the active profile. No-op if the
 * player has no real (non-default) active profile. */

#include <stdint.h>
#include "headers/player_profile.h"

extern int16_t main_get_current_solo_level(void);
extern int16_t game_difficulty_level_get(void);
extern int player_ui_get_active_player_profile_index(int16_t local_player_index);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern int player_profile_write(int index, player_profile *profile);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);

void player_profile_save_level_completed(int16_t local_player_index)
{
    short current_solo_level = main_get_current_solo_level();
    char difficulty = game_difficulty_level_get();
    int active_player_profile_index = player_ui_get_active_player_profile_index(local_player_index);

    if ( active_player_profile_index != -1 )
    {
        player_profile profile;
        player_ui_get_active_player_profile(local_player_index, &profile);
        profile.single_player_map_flags[current_solo_level] |= 1 << difficulty;
        player_profile_write(active_player_profile_index, &profile);
        player_ui_set_active_player_profile(local_player_index, active_player_profile_index, &profile);
    }
}
