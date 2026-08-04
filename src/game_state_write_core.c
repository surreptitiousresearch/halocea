/* game_state_write_core @0x8371B208 — create the core directory if needed and write buffer_size bytes to a
 * named core file (overwriting). Returns true only on a full write. */

#include <stdint.h>
#include "headers/xbox_game_state_globals.h"
#include <windows.h>

extern int sprintf_0(char *string, const char *format, ...);

uint8_t game_state_write_core(const char *name, const void *buffer, unsigned int buffer_size)
{
    uint8_t succeeded = 0;
    char path[1072];
    unsigned int bytes_written;

    CreateDirectoryA(xbox_game_state_globals.core_path, 0);
    sprintf_0(path, "%s\\%s", xbox_game_state_globals.core_path, name);
    void *file = CreateFileA(path, 0x40000000u, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if ( file != INVALID_HANDLE_VALUE && WriteFile(file, buffer, buffer_size, &bytes_written, 0) )
        succeeded = bytes_written == buffer_size;
    GetLastError();
    CloseHandle(file);
    return succeeded;
}
