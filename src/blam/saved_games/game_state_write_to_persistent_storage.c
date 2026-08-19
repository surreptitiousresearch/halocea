/* game_state_write_to_persistent_storage @0x8371B818 — write the game state buffer to the persistent
 * storage file: checksums the whole buffer's fixed leading region (4489216 bytes) into
 * *header_checksum, backs up the header portion, zeroes it in the live buffer, then writes the
 * (now header-zeroed) buffer followed by the real header separately. If any step of that fails, deletes
 * the local player's save file as a corruption guard. Always restores the header into the live buffer
 * before returning (whether or not the write succeeded) and closes the storage handle. No-op if the
 * storage file couldn't be opened. */

#include <stdint.h>
#include <string.h>
#include "headers/_OVERLAPPED.h"

extern void *game_state_open_persistent_storage(const char *directory);
extern void crc_new(uint32_t *crc_reference);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);
/* memset declared by <string.h> */
extern unsigned int SetFilePointer(void *hFile, int lDistanceToMove, int *lpDistanceToMoveHigh,
    unsigned int dwMoveMethod);
extern int WriteFile(void *hFile, const void *lpBuffer, unsigned int nNumberOfBytesToWrite,
    unsigned int *lpNumberOfBytesWritten, _OVERLAPPED *lpOverlapped);
extern uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path);
extern int DeleteFileA(const char *lpFileName);
extern int CloseHandle(void *hObject);

void game_state_write_to_persistent_storage(void *buffer, unsigned int *header_checksum, int header_size,
    int buffer_size)
{
    void *file = (void *)(intptr_t)game_state_open_persistent_storage(nullptr);
    if ( file == (void *)-1 )
        return;

    *header_checksum = 0;

    unsigned int crc[3];
    crc_new(crc);
    crc_checksum_buffer(crc, buffer, 4489216);
    *header_checksum = crc[0];

    char header_backup[2096];
    memcpy(header_backup, buffer, header_size);
    memset(buffer, 0, header_size);

    unsigned int bytes_transferred;
    if ( SetFilePointer(file, 0, nullptr, 0) == -1
      || !WriteFile(file, buffer, buffer_size, &bytes_transferred, nullptr)
      || bytes_transferred != (unsigned int)buffer_size
      || SetFilePointer(file, 0, nullptr, 0) == -1
      || !WriteFile(file, header_backup, header_size, &bytes_transferred, nullptr)
      || bytes_transferred != (unsigned int)header_size )
    {
        char profile_directory[256];
        if ( player_ui_get_path_to_local_player_profile_directory(0, profile_directory) )
            DeleteFileA(profile_directory);
    }

    memcpy(buffer, header_backup, header_size);
    CloseHandle(file);
}
