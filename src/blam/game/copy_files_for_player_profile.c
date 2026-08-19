/* copy_files_for_player_profile @0x83726D90 — copy every saved file from one player profile directory to another
 * (sibling of delete_files_for_player_profile). Copies "<src>blam.sav" -> "<dst>blam.sav" first; on success copies
 * "<src><persistent-storage-file>" -> "<dst>...", then a sibling of each with the extension replaced by ".sav". If
 * that last copy succeeded it copies the checkpoint directory: every "<src>checkpoints\*.sav" then every
 * "<src>checkpoints\*.bin" into the destination, stopping a sweep at the first CopyFileA that fails. The return
 * value reflects only the blam.sav + persistent-storage copies (the ".sav" variant and checkpoints gate further
 * work but do not change the result).
 *
 * The ".sav" append after truncating the extension is an inlined strcat (byte-copy including the terminator),
 * applied to both the source and destination paths. */

#include <stdint.h>
#include "headers/win32_find_data.h"

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern int CopyFileA(const char *existing_filename, const char *new_filename, int fail_if_exists);
extern const char * game_state_get_persistent_storage_filename(void);
extern char *strrchr(const char *string, int character);
extern void *FindFirstFileA(const char *pattern, WIN32_FIND_DATAA *find_data);
extern int FindNextFileA(void *find_handle, WIN32_FIND_DATAA *find_data);
extern int CloseHandle(void *handle);

static void replace_extension_with_sav(char *path)
{
    char *extension = strrchr(path, '.');
    if ( extension )
        *extension = 0;

    char *end = path;
    while ( *end++ )
        ;
    --end;
    const char *append = ".sav";
    int character;
    do
    {
        character = *(uint8_t *)append++;
        *end++ = character;
    }
    while ( character );
}

uint8_t copy_files_for_player_profile(const char *src_profile, const char *dst_profile)
{
    char dst_path[256];
    char src_path[256];
    WIN32_FIND_DATAA find_data;
    char pattern[320];

    _snprintf_0(dst_path, 0xFFu, "%s%s", dst_profile, "blam.sav");
    _snprintf_0(src_path, 0xFFu, "%s%s", src_profile, "blam.sav");
    src_path[255] = 0;
    dst_path[255] = 0;

    int copied = (uint8_t)CopyFileA(src_path, dst_path, 0);
    int variant_copied = 0;
    if ( copied )
    {
        const char *persistent_storage_filename = game_state_get_persistent_storage_filename();
        _snprintf_0(dst_path, 0xFFu, "%s%s", dst_profile, persistent_storage_filename);
        persistent_storage_filename = game_state_get_persistent_storage_filename();
        _snprintf_0(src_path, 0xFFu, "%s%s", src_profile, persistent_storage_filename);
        src_path[255] = 0;
        dst_path[255] = 0;

        copied = (uint8_t)CopyFileA(src_path, dst_path, 0);
        if ( copied )
        {
            replace_extension_with_sav(dst_path);
            replace_extension_with_sav(src_path);
            variant_copied = CopyFileA(src_path, dst_path, 0);
        }
    }

    variant_copied = (uint8_t)variant_copied;
    if ( variant_copied )
    {
        _snprintf_0(pattern, 0xFFu, "%scheckpoints\\*.sav", src_profile);
        void *find_handle = FindFirstFileA(pattern, &find_data);
        if ( find_handle != (void *)-1 )
        {
            do
            {
                _snprintf_0(dst_path, 0xFFu, "%scheckpoints\\%s", dst_profile, find_data.cFileName);
                _snprintf_0(src_path, 0xFFu, "%scheckpoints\\%s", src_profile, find_data.cFileName);
                variant_copied = (uint8_t)CopyFileA(src_path, dst_path, 0);
            }
            while ( variant_copied && FindNextFileA(find_handle, &find_data) );
            CloseHandle(find_handle);
        }

        if ( variant_copied )
        {
            _snprintf_0(pattern, 0xFFu, "%scheckpoints\\*.bin", src_profile);
            void *bin_handle = FindFirstFileA(pattern, &find_data);
            if ( bin_handle != (void *)-1 )
            {
                do
                {
                    _snprintf_0(dst_path, 0xFFu, "%scheckpoints\\%s", dst_profile, find_data.cFileName);
                    _snprintf_0(src_path, 0xFFu, "%scheckpoints\\%s", src_profile, find_data.cFileName);
                }
                while ( (uint8_t)CopyFileA(src_path, dst_path, 0) && FindNextFileA(bin_handle, &find_data) );
                CloseHandle(bin_handle);
            }
        }
    }

    return copied;
}
