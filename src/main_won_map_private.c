/* main_won_map_private @0x8368A5C8 — commit a level-won transition: return to the main menu, record the
 * completed level in every spawned player's profile, and queue the next solo level (clamped: solo id+1, or
 * -1 once past the last level at index 10). */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

extern int16_t main_get_solo_level_from_name(const char *name);
extern void player_profile_save_level_completed(int16_t local_player_index);
extern void ui_set_next_level(int16_t level);

void main_won_map_private(void)
{
    main_globals.want_to_be_at_main_menu = 1;
    main_globals.won_map = 0;

    __int16 next_level = (__int16)(main_get_solo_level_from_name(main_globals.soloplayer_map_name) + 1);
    if ( next_level >= 10 )
        next_level = -1;

    for ( __int16 player_index = 0; player_index < player_spawn_count; ++player_index )
        player_profile_save_level_completed(player_index);

    ui_set_next_level(next_level);
}
