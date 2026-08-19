/* game_state_read_core_header @0x8371B2C0 — read the first header_size bytes of a named core file into header.
 * Returns true only if a full header_size-byte read succeeded. */

#include <stdint.h>
#include "headers/xbox_game_state_globals.h"
#include <windows.h>

extern int sprintf_0(char *string, const char *format, ...);

uint8_t game_state_read_core_header(const char *name, void *header, unsigned int header_size)
{
    uint8_t succeeded = 0;
    char path[1072];
    unsigned int bytes_read;

    sprintf_0(path, "%s\\%s", xbox_game_state_globals.core_path, name);
    void *file = CreateFileA(path, 0x80000000, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if ( file != INVALID_HANDLE_VALUE && ReadFile(file, header, header_size, &bytes_read, 0) )
        succeeded = bytes_read == header_size;
    CloseHandle(file);
    return succeeded;
}
