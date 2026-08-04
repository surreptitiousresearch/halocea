/* game_state_read_from_persistent_storage @0x8371B988 — read the saved game-state arena back from its
 * persistent-storage file into buffer. On any failure (open, seek, short read), the local player's profile
 * directory copy is deleted so the corrupt save isn't reused. */

#include <stdint.h>

/* def (src/game_state_open_persistent_storage.c) returns `int` (a file handle carried as int) and takes
 * `const char *directory`; matched here. */
extern void *game_state_open_persistent_storage(const char *directory);
extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern int ReadFile(void *file, void *buffer, unsigned int count, unsigned int *read, void *overlapped);
extern int CloseHandle(void *file);
extern int DeleteFileA(const char *path);
extern uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path);

void game_state_read_from_persistent_storage(void *buffer, unsigned int buffer_size)
{
    void *file = game_state_open_persistent_storage(0);
    if ( file != (void *)-1 )
    {
        unsigned int bytes_read;
        if ( SetFilePointer(file, 0, 0, 0) == -1
          || !ReadFile(file, buffer, buffer_size, &bytes_read, 0)
          || bytes_read != buffer_size )
        {
            char profile_path[288];
            if ( player_ui_get_path_to_local_player_profile_directory(0, profile_path) )
                DeleteFileA(profile_path);
        }
        CloseHandle(file);
    }
}
