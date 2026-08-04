/* player_ui_get_path_to_local_player_profile_directory @0x83698C10 — resolve the on-disk profile directory
 * path for a local player (0 or 1) from its active profile index. Returns 0 for an out-of-range index.
 * The profile index is read from player_ui_globals.player_data[idx].profile_index (@1980). */

#include <stdint.h>
#include "headers/player_ui_globals.h"

extern uint8_t player_profile_get_enclosing_directory_path(int player_profile_index, char *full_path);

uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path)
{
    if ( (unsigned int)local_player_index > 1 )
        return 0;
    return player_profile_get_enclosing_directory_path(
        player_ui_globals.player_data[local_player_index].profile_index,
        full_path);
}
