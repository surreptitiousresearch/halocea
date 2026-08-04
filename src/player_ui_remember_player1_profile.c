/* player_ui_remember_player1_profile @0x83698C40 — cache player 1's active profile directory path,
 * updating it when the selected profile changes, and optionally persist it to disk. */

#include <stdint.h>
#include "headers/player_ui_player_data.h"
#include "headers/blam_data_globals.h"


extern uint8_t player_profile_get_enclosing_directory_path(int player_profile_index, char *full_path);
extern void saved_game_file_remember_player1_last_used_profile_directory(char *directory_path);

void player_ui_remember_player1_profile(uint8_t write_to_disk)
{
    int profile_index = player_ui_globals.player_data[0].profile_index;

    if ( player1_last_used_profile_index != profile_index )
    {
        if ( profile_index != -1 )
        {
            player_profile_get_enclosing_directory_path(profile_index, player1_profile_path);
            profile_index = player_ui_globals.player_data[0].profile_index;
        }
        player1_last_used_profile_index = profile_index;
    }

    if ( write_to_disk && player1_profile_path[0] )
        saved_game_file_remember_player1_last_used_profile_directory(player1_profile_path);
}
