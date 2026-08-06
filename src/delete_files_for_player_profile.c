/* delete_files_for_player_profile @0x83726B90 — remove every saved file belonging to one player profile
 * directory. Deletes "<profile>blam.sav" first; on success deletes "<profile><persistent-storage-file>", then
 * a sibling of it with the extension replaced by ".sav". If all of those succeeded it clears the profile's
 * checkpoint directory: every "<profile>checkpoints\*.sav" then every "<profile>checkpoints\*.bin", stopping a
 * sweep at the first DeleteFileA that fails. Returns nonzero only if nothing failed along the way.
 *
 * The ".sav" append after truncating the extension is an inlined strcat (byte-copy including the terminator). */

#include <stdint.h>
#include "headers/win32_find_data.h"

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern int DeleteFileA(const char *filename);
extern const char * game_state_get_persistent_storage_filename(void);
extern char *strrchr(const char *string, int character);
extern void *FindFirstFileA(const char *pattern, WIN32_FIND_DATAA *find_data);
extern int FindNextFileA(void *find_handle, WIN32_FIND_DATAA *find_data);
extern int CloseHandle(void *handle);

int delete_files_for_player_profile(const char *profile)
{
    char path[256];
    WIN32_FIND_DATAA find_data;
    char pattern[304];

    _snprintf_0(path, 0xFFu, "%s%s", profile, "blam.sav");
    path[255] = 0;
    uint8_t ok = (uint8_t)DeleteFileA(path);
    if ( ok )
    {
        const char *persistent_storage_filename = game_state_get_persistent_storage_filename();
        _snprintf_0(path, 0xFFu, "%s%s", profile, persistent_storage_filename);
        path[255] = 0;
        ok = (uint8_t)DeleteFileA(path);
        if ( ok )
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

            ok = (uint8_t)DeleteFileA(path);
        }
    }

    uint8_t sweep_ok = ok;
    if ( ok )
    {
        _snprintf_0(pattern, 0xFFu, "%scheckpoints\\*.sav", profile);
        void *find_handle = FindFirstFileA(pattern, &find_data);
        if ( find_handle != (void *)-1 )
        {
            do
            {
                _snprintf_0(path, 0xFFu, "%scheckpoints\\%s", profile, find_data.cFileName);
                ok = (uint8_t)DeleteFileA(path);
                sweep_ok = ok;
            }
            while ( ok && FindNextFileA(find_handle, &find_data) );
            CloseHandle(find_handle);
        }

        if ( sweep_ok )
        {
            _snprintf_0(pattern, 0xFFu, "%scheckpoints\\*.bin", profile);
            void *bin_handle = FindFirstFileA(pattern, &find_data);
            if ( bin_handle != (void *)-1 )
            {
                do
                {
                    _snprintf_0(path, 0xFFu, "%scheckpoints\\%s", profile, find_data.cFileName);
                    ok = (uint8_t)DeleteFileA(path);
                }
                while ( ok && FindNextFileA(bin_handle, &find_data) );
                CloseHandle(bin_handle);
            }
        }
    }

    return ok;
}
