/* hcex_checkpoint_read_header @0x83684ED8 — open a checkpoint/save file, read its 332-byte game_state_header
 * and report whether it is valid for the running game. Returns 1 only when the file opens, the full header
 * reads, and game_state_header_valid accepts it; 0 otherwise.
 *
 * DEVIATION: the disassembly passes a stale register as game_state_header_valid's 2nd argument (the
 * "corrupted" out-flag) — it is never set by this caller, so it is written as 0 here. */

#include <stdint.h>
#include "../headers/game_state_header.h"

extern "C" void *CreateFileA(const char *file_name, unsigned int desired_access, unsigned int share_mode,
                         void *security_attributes, unsigned int creation_disposition,
                         unsigned int flags_and_attributes, void *template_file);
extern "C" int ReadFile(void *file, void *buffer, unsigned int count, unsigned int *read, void *overlapped);
extern "C" int CloseHandle(void *handle);
extern "C" uint8_t game_state_header_valid(game_state_header *header, uint8_t fatal);

#define GENERIC_READ          0x80000000u
#define OPEN_EXISTING         3u
#define FILE_ATTRIBUTE_NORMAL 0x80u
#define INVALID_HANDLE_VALUE  ((void *)-1)

extern "C" int hcex_checkpoint_read_header(const char *filename)
{
    int valid = 0;
    void *file = CreateFileA(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    game_state_header header;
    unsigned int bytes_read;
    if ( file != INVALID_HANDLE_VALUE
        && ReadFile(file, &header, sizeof(header), &bytes_read, 0)
        && bytes_read == sizeof(header) )
    {
        valid = game_state_header_valid(&header, 0) != 0;
    }

    CloseHandle(file);
    return valid;
}
