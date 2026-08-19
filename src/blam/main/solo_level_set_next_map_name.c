/* solo_level_set_next_map_name @0x83691D00 — widget callback: decide whether the selected solo-campaign
 * level in the list is unlocked for the active player(s) (already completed, one past the highest
 * completed level, or the first level), and if so start loading it. With 2 local players, any one of
 * them unlocking the level is enough. hcex_is_unlock_all_maps overrides everything. Plays an error sound
 * and returns false if the level isn't unlocked. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"


extern int preview_list_get_list_index_for_item(int index);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void player_profile_get_highest_completed_solo_level(player_profile *profile, int16_t *level, int16_t *difficulty);
extern void player_ui_remember_player1_profile(uint8_t write_to_disk);
extern void main_set_map_name(const char *name);
extern void main_defer_map_map_change(void);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t solo_level_set_next_map_name(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    int unlocked = 0;
    int list_index = preview_list_get_list_index_for_item(list_widget->parameters.list_parameters.selected_list_item_index);

    if ( player_spawn_count == 1 )
    {
        player_profile profile;
        int16_t highest_level, highest_difficulty[7];

        player_ui_get_active_player_profile(0, &profile);
        player_profile_get_highest_completed_solo_level(&profile, &highest_level, highest_difficulty);

        if ( profile.single_player_map_flags[list_index] || list_index == highest_level + 1 || !list_index )
            unlocked = 1;

        player_ui_remember_player1_profile(0);
    }
    else if ( player_spawn_count == 2 )
    {
        for ( int16_t local_player_index = 0; ; local_player_index = (int16_t)(local_player_index + 1) )
        {
            player_profile profile;
            int16_t highest_level, highest_difficulty[7];

            player_ui_get_active_player_profile(local_player_index, &profile);
            player_profile_get_highest_completed_solo_level(&profile, &highest_level, highest_difficulty);

            if ( profile.single_player_map_flags[list_index] || list_index == highest_level + 1 || !list_index )
            {
                unlocked = 1;
                break;
            }

            if ( local_player_index > 1 )
                break;
        }
    }

    if ( hcex_is_unlock_all_maps )
        unlocked = 1;

    if ( (uint8_t)unlocked == 1 )
    {
        main_set_map_name(scenario_paths_0[list_index]);
        main_defer_map_map_change();
    }
    else
    {
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    }

    return unlocked;
}
