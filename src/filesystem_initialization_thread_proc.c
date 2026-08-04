#include <stdint.h>
#include "headers/widget_globals.h"

extern int16_t saved_game_perform_file_system_checks(void);
extern void playlist_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *playlist_profile_indices);
extern void player_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);
extern int player_ui_get_player1_last_used_profile_index(void);

unsigned int filesystem_initialization_thread_proc(void *input)
{
    widget_globals.filesystem_check_result = saved_game_perform_file_system_checks();
    if ( !widget_globals.filesystem_check_result )
    {
        unsigned __int16 number_of_profiles = 1;
        int profile_index;

        playlist_profiles_enumerate_available_to_local_player_index(-1, &number_of_profiles, &profile_index);
        player_profiles_enumerate_available_to_local_player_index(-1, &number_of_profiles, &profile_index, 1u);
        player_ui_get_player1_last_used_profile_index();
    }
    return 0;
}
