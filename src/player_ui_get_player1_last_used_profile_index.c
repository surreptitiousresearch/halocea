#include <stdint.h>
#include "headers/blam_data_globals.h"
/* player_ui_get_player1_last_used_profile_index @0x83698CB8 — resolve (and cache) the saved-profile
 * index for player 1's last-used profile directory. On first call (path still empty) it retrieves the
 * directory path from the save system and looks up its non-multiplayer profile index; afterwards it
 * returns the cached index. */


extern uint8_t saved_game_file_retrieve_player1_last_used_profile_directory(char *directory_path);
extern int saved_game_file_find_profile_index_for_directory_path(char *directory_path, int16_t saved_game_file_type);

int player_ui_get_player1_last_used_profile_index(void)
{
    if ( player1_profile_path[0] || !saved_game_file_retrieve_player1_last_used_profile_directory(player1_profile_path) )
        return player1_last_used_profile_index;

    player1_last_used_profile_index = saved_game_file_find_profile_index_for_directory_path(player1_profile_path, 0);
    return player1_last_used_profile_index;
}
