#include <stdint.h>

extern uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path);
extern int DeleteFileA(const char *path);

int delete_persistent_storage(void)
{
    char path[264];

    if (!player_ui_get_path_to_local_player_profile_directory(0, path))
        return 0;

    return DeleteFileA(path);
}
