/* game_state_open_persistent_storage @0x8371B480 — opens (creating if needed) the "savegame.bin"
 * persistent-storage file, either in the caller-supplied directory or, when directory is null, in the
 * local player's profile directory. Pre-sizes a freshly created file to 4.5 MB (writing a zeroed 16 KB
 * first block, then extending with SetFilePointer/SetEndOfFile); if pre-sizing fails, the just-fetched
 * profile-directory path is deleted. Returns the open file handle, or INVALID_HANDLE_VALUE (-1) on
 * failure. */

#include <stdint.h>
#include <string.h>

extern uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path);
extern void *CreateFileA(const char *name, unsigned int access, unsigned int share, void *security,
    unsigned int creation, unsigned int flags, void *template_file);
extern unsigned int GetFileSize(void *file, unsigned int *high);
extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern int SetEndOfFile(void *file);
extern int WriteFile(void *file, const void *buffer, unsigned int count, unsigned int *written, void *overlapped);
extern int DeleteFileA(const char *path);
extern int CloseHandle(void *file);

void *game_state_open_persistent_storage(const char *directory)
{
    char path[256];
    void *file = (void *)-1;
    unsigned char sized_ok = 0;
    unsigned char have_path = 1;

    if (directory)
    {
        strcpy(path, directory);
    }
    else
    {
        have_path = player_ui_get_path_to_local_player_profile_directory(0, path);
        if (have_path)
        {
            /* binary calls the path fetch a second time (0x8371B4E0) after testing the first
             * call's result — kept binary-faithful */
            player_ui_get_path_to_local_player_profile_directory(0, path);
        }
    }

    if (have_path)
    {
        strcat(path, "savegame.bin");
        file = CreateFileA(path, 0xC0000000, 0, nullptr, 4u, 0, nullptr);

        if (file != (void *)-1)
        {
            if (GetFileSize(file, nullptr) == 4718592)
            {
                sized_ok = 1;
            }
            else
            {
                unsigned char zero_block[0x4000];
                memset(zero_block, 0, sizeof(zero_block));

                unsigned int written;
                if (WriteFile(file, zero_block, sizeof(zero_block), &written, nullptr)
                        && written == sizeof(zero_block)
                        && SetFilePointer(file, 4718592, nullptr, 0) != (unsigned int)-1
                        && SetEndOfFile(file))
                {
                    sized_ok = 1;
                }
                else
                {
                    /* BINARY BUG (0x8371B600..0x8371B620): deletes the bare profile-directory
                     * path — "savegame.bin" is never appended before DeleteFileA. Kept faithful. */
                    char profile_path[256];
                    if (player_ui_get_path_to_local_player_profile_directory(0, profile_path))
                        DeleteFileA(profile_path);
                }
            }
        }
    }

    if (sized_ok)
        return file;
    if (file != (void *)-1)
        CloseHandle(file);
    return (void *)-1;
}
