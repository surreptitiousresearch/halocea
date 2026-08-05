/* XDeleteSaveGame @0x83818028 — the HCEX PC-port reimplementation of the Xbox save-game deletion API. Given a
 * root path and a (wide) save-game name, it deletes the save's directory "<root>\<name>\" and its "checkpoints"
 * subdirectory. First it deletes every file directly in the save directory except "." / ".." and the
 * "checkpoints" folder, stopping at the first DeleteFileA that fails. If that succeeded it deletes every file
 * in "<save>\checkpoints\" (again skipping "."/".."/"checkpoints"), removes the now-empty checkpoints
 * directory, and finally — after trimming the trailing backslash — removes the save directory itself. Returns
 * 87 (ERROR_INVALID_PARAMETER) for null arguments, otherwise 0 on success and 1 on failure.
 *
 * DEVIATION: the compiler inlined the two strcmp(cFileName, "checkpoints") comparisons into byte loops; they
 * are restored to strcmp here. The trailing-backslash trim walks to the NUL then clears the byte before it. */

#include <stdint.h>
#include "headers/win32_find_data.h"

extern char * wide_to_ascii(uint16_t *unicode, char *ascii, unsigned int ascii_length_bytes);
extern int sprintf_0(char *string, const char *format, ...);
extern int strcmp(const char *string1, const char *string2);
extern void *FindFirstFileA(const char *pattern, _WIN32_FIND_DATAA *find_data);
extern int FindNextFileA(void *find_handle, _WIN32_FIND_DATAA *find_data);
extern int CloseHandle(void *handle);
extern int DeleteFileA(const char *filename);
extern int RemoveDirectoryA(const char *path);

unsigned int XDeleteSaveGame(const char *lpRootPathName, const wchar_t *lpSaveGameName)
{
    if ( !lpRootPathName || !lpSaveGameName )
        return 87;

    char save_name[128];
    wide_to_ascii((uint16_t *)lpSaveGameName, save_name, 0x80u);

    char save_directory[272];
    sprintf_0(save_directory, "%s\\%s\\", lpRootPathName, save_name);

    char pattern[304];
    sprintf_0(pattern, "%s*.*", save_directory);

    int result = 0;
    _WIN32_FIND_DATAA find_data;
    void *find_handle = FindFirstFileA(pattern, &find_data);
    if ( find_handle != (void *)-1 )
    {
        do
        {
            if ( find_data.cFileName[0] != '.' && strcmp(find_data.cFileName, "checkpoints") != 0 )
            {
                sprintf_0(save_name, "%s%s", save_directory, find_data.cFileName);
                result = DeleteFileA(save_name);
                if ( !result )
                    break;
            }
        }
        while ( FindNextFileA(find_handle, &find_data) );
        CloseHandle(find_handle);
    }

    sprintf_0(pattern, "%scheckpoints\\*.*", save_directory);
    if ( result )
    {
        void *checkpoint_handle = FindFirstFileA(pattern, &find_data);
        if ( checkpoint_handle != (void *)-1 )
        {
            do
            {
                if ( find_data.cFileName[0] != '.' && strcmp(find_data.cFileName, "checkpoints") != 0 )
                {
                    sprintf_0(save_name, "%scheckpoints\\%s", save_directory, find_data.cFileName);
                    DeleteFileA(save_name);
                }
            }
            while ( FindNextFileA(checkpoint_handle, &find_data) );
            CloseHandle(checkpoint_handle);

            sprintf_0(pattern, "%scheckpoints", save_directory);
            result = RemoveDirectoryA(pattern);
        }

        if ( result )
        {
            char *end = save_directory;
            while ( *end++ )
                ;
            save_directory[end - save_directory - 2] = 0;   /* drop trailing '\\' */
            result = RemoveDirectoryA(save_directory);
        }
    }

    return -1 - ((result == 1) - 2);
}
