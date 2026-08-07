/* player_ui_initialize @ 0x83699C50 — build default profiles for the local players
 * and select player 1's last-used (or first available) profile. */

#include <stdint.h>
#include <string.h>
#include "headers/player_ui_globals.h"
#include "headers/blam_data_globals.h"

extern void player_profile_build_default_profile(player_profile *profile, int player_index, uint8_t try_inherit);
extern void set_local_player_controls_from_player_profile(int16_t local_player_index);
extern void player_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);
extern void saved_game_files_notify_memory_units_changed(void);
extern uint8_t saved_game_file_retrieve_player1_last_used_profile_directory(char *directory_path);
extern int saved_game_file_find_profile_index_for_directory_path(char *directory_path, int16_t saved_game_file_type);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern int player_profile_load_default_video_settings(player_profile *profile, uint8_t try_inherit);
extern int player_profile_load_default_audio_settings(player_profile *profile, uint8_t try_inherit);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);
extern void player_profile_save(int player_profile_index, player_profile *profile);


void player_ui_initialize(void)
{
    int i;
    unsigned short available_count;   /* the enumerate out-param is uint16_t*; typed to match (drops the pointer cast) */
    int available_indices[3];
    player_profile profile;
    int profile_index;

    memset(&player_ui_globals, 0, sizeof(player_ui_globals));

    for ( i = 0; i < 2; ++i )
    {
        player_profile_build_default_profile(&player_ui_globals.player_data[i].profile, 0, 0);
        player_ui_globals.player_data[i].profile_index = -1;
        player_ui_globals.single_player_controller_index[(int16_t)i] = -1;
        set_local_player_controls_from_player_profile(i);
    }

    available_indices[0] = -1;
    player_ui_globals.edit_profile_data.profile_index = -1;
    available_count = 1;
    player_profiles_enumerate_available_to_local_player_index(
        -1, &available_count, available_indices, 0);
    saved_game_files_notify_memory_units_changed();

    if ( player1_profile_path[0]
      || !saved_game_file_retrieve_player1_last_used_profile_directory(player1_profile_path) )
    {
        profile_index = player1_last_used_profile_index;
    }
    else
    {
        profile_index = saved_game_file_find_profile_index_for_directory_path(player1_profile_path, 0);
        player1_last_used_profile_index = profile_index;
    }

    if ( !player_profile_get(profile_index, &profile) )
    {
        if ( available_count == 0 || available_indices[0] == -1
          || !player_profile_get(available_indices[0], &profile) )
        {
            player_ui_globals.initialized = 1;
            return;
        }
        profile_index = available_indices[0];
    }

    if ( profile_index != -1 )
    {
        if ( optionSafe )
        {
            player_profile_load_default_video_settings(&profile, 0);
            player_profile_load_default_audio_settings(&profile, 0);
        }
        player_ui_set_active_player_profile(0, profile_index, &profile);
        if ( optionSafe )
            player_profile_save(available_indices[0], &profile);
    }
    player_ui_globals.initialized = 1;
}
