/* game_state_read_core @0x8371B368 — read a named core file (under the core path) into a buffer. Best-effort:
 * a missing file is silently ignored. */

#include "headers/xbox_game_state_globals.h"
#include <windows.h>

extern int sprintf_0(char *string, const char *format, ...);

void game_state_read_core(const char *name, void *buffer, unsigned int buffer_size)
{
    char path[1056];
    unsigned int bytes_read;

    sprintf_0(path, "%s\\%s", xbox_game_state_globals.core_path, name);
    void *file = CreateFileA(path, 0x80000000, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if ( file != INVALID_HANDLE_VALUE )
        ReadFile(file, buffer, buffer_size, &bytes_read, 0);
    CloseHandle(file);
}
