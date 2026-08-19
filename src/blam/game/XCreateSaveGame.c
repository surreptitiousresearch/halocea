/* XCreateSaveGame @0x83817D98 — Xbox save-game creation shim implemented over the Win32 file API (sibling of
 * XDeleteSaveGame). Validates its arguments, converts the wide save name to ASCII, and builds the save directory,
 * per-save info-file path, and save-root path. It ensures the root directory exists, then dispatches on
 * dwCreationDisposition: 1 or 4 create the save (make the save + "checkpoints" subdirectories if new, write the
 * info file, and copy the save directory back to lpPathBuffer); 3 opens an existing save (fail if it is not
 * there); anything else is an invalid parameter. Returns 0 on success, 87 (ERROR_INVALID_PARAMETER) for bad
 * arguments/disposition, or 0x80004005 (E_FAIL) on any filesystem failure.
 *
 * The inlined strcpy/strcat/strlen byte loops Hex-Rays emitted are written here as the library calls they came
 * from. The "Name=%s" metadata is converted to wide (metadata_wide) but not otherwise used — reproduced as-is
 * (the write path stores the info-file path text, not the metadata). dwCreateFlags is unused. */

#include <stdint.h>
#include <string.h>

extern char * wide_to_ascii(uint16_t *unicode, char *ascii, unsigned int ascii_length_bytes);
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern int sprintf_0(char *string, const char *format, ...);

extern unsigned int GetFileAttributesA(const char *filename);
extern int CreateDirectoryA(const char *path, void *security_attributes);
extern void *CreateFileA(const char *filename, unsigned int desired_access, unsigned int share_mode,
        void *security_attributes, unsigned int creation_disposition, unsigned int flags_and_attributes,
        void *template_file);
extern int WriteFile(void *file, const void *buffer, unsigned int bytes_to_write,
        unsigned int *bytes_written, void *overlapped);
extern int CloseHandle(void *handle);

unsigned int XCreateSaveGame(const char *lpRootPathName, const wchar_t *lpSaveGameName,
        unsigned int dwCreationDisposition, unsigned int dwCreateFlags, char *lpPathBuffer, unsigned int uSize)
{
    int exists = 0;
    if ( !lpRootPathName || !lpSaveGameName || !lpPathBuffer || !uSize )
        return 87;

    char save_name[128];
    wide_to_ascii((uint16_t *)lpSaveGameName, save_name, 0x80);

    char save_dir[272];
    char info_file_path[272];
    sprintf_0(save_dir, "%s\\%s\\", lpRootPathName, save_name);
    sprintf_0(info_file_path, "%s%s", save_dir, save_name);

    char metadata[1072];
    uint16_t metadata_wide[128];
    sprintf_0(metadata, "Name=%s\n", save_name);
    ascii_to_wide(metadata, metadata_wide, 0x80);

    char save_path[272];
    sprintf_0(save_path, "%s\\%s", lpRootPathName, save_name);

    if ( GetFileAttributesA(lpRootPathName) == 0xFFFFFFFF && !CreateDirectoryA(lpRootPathName, nullptr) )
        return 0x80004005;
    if ( GetFileAttributesA(save_path) != 0xFFFFFFFF )
        exists = 1;

    if ( dwCreationDisposition == 1 || dwCreationDisposition == 4 )
    {
        if ( !exists )
        {
            if ( !CreateDirectoryA(save_dir, nullptr) )
                return 0x80004005;
            char checkpoints_dir[272];
            strcpy(checkpoints_dir, save_dir);
            strcat(checkpoints_dir, "checkpoints\\");
            if ( !CreateDirectoryA(checkpoints_dir, nullptr) )
                return 0x80004005;
        }

        void *file = CreateFileA(info_file_path, 0xC0000000, 0, nullptr, 2, 0x80, nullptr);
        if ( file == (void *)-1 )
            return 0x80004005;

        unsigned int length = strlen(info_file_path);
        unsigned int bytes_written = 0;
        WriteFile(file, info_file_path, length, &bytes_written, nullptr);
        CloseHandle(file);
        if ( length != bytes_written )
            return 0x80004005;

        strncpy(lpPathBuffer, save_dir, uSize);
        return 0;
    }

    if ( dwCreationDisposition == 3 )
    {
        if ( !exists )
            return 0x80004005;
        void *file = CreateFileA(info_file_path, 0, 0, nullptr, 2, 0x80, nullptr);
        if ( file == (void *)-1 )
            return 0x80004005;
        CloseHandle(file);
        return 0;
    }

    return 87;
}
