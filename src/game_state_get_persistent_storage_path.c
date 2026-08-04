#include <stdint.h>

extern uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path);

uint8_t game_state_get_persistent_storage_path(char *path)
{
    return player_ui_get_path_to_local_player_profile_directory(0, path) != 0;
}
