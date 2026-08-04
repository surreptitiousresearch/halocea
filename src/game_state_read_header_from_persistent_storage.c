/* game_state_read_header_from_persistent_storage @0x8371B688 — read and validate the saved-game header
 * from the local player's persistent-storage file. Opens the file, reads `header_size` bytes into `header`,
 * then verifies the whole file's CRC: the checksum stored in the header (at *header_checksum) is captured,
 * the field is zeroed, and a CRC is accumulated over the header followed by the remaining file in 128 KB
 * chunks (calling sound_idle between chunks so audio keeps mixing during the blocking read). Returns 1 if
 * the computed CRC matches the stored one. On mismatch with a non-zero stored checksum, *corrupted is set.
 * If the file cannot be read or is the wrong size, the corrupt profile file is deleted. Returns 0 on any
 * failure.
 *
 * DEVIATION: the decompiler inflated this to 31 args and a 64-bit packed local because _RtlCheckStack12
 * (the large-frame stack probe for the 128 KB read buffer) clobbered its register/stack tracking. The DB
 * prototype (5 args) is ground truth; the real register assignment was recovered from the disassembly:
 * r3=header, r4=header_checksum, r5=header_size, r6=buffer_size, r7=corrupted. */

#include <stdint.h>

extern void *game_state_open_persistent_storage(const char *directory);
extern void crc_new(uint32_t *crc_reference);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);
extern void sound_idle(void);
extern uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path);

extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern int  ReadFile(void *file, void *buffer, unsigned int count, unsigned int *read, void *overlapped);
extern int  CloseHandle(void *handle);
extern int  DeleteFileA(const char *path);

#define INVALID_HANDLE_VALUE ((void *)-1)
#define READ_CHUNK_SIZE      0x20000

uint8_t game_state_read_header_from_persistent_storage(void *header, unsigned int *header_checksum, int header_size, int buffer_size, uint8_t *corrupted)
{
    void *file = game_state_open_persistent_storage(0);

    if (corrupted)
        *corrupted = 0;

    if (file == INVALID_HANDLE_VALUE)
        return 0;

    if (SetFilePointer(file, 0, 0, 0) != (unsigned int)-1)
    {
        unsigned int bytes_read;
        if (ReadFile(file, header, header_size, &bytes_read, 0)
            && bytes_read == (unsigned int)header_size)
        {
            unsigned int stored_checksum = *header_checksum;
            unsigned int crc_reference;
            crc_new(&crc_reference);
            *header_checksum = 0; /* the embedded checksum field must be zero while checksumming */
            crc_checksum_buffer(&crc_reference, header, header_size);

            char read_buffer[READ_CHUNK_SIZE];
            int remaining = buffer_size - header_size;
            while (remaining > 0)
            {
                int chunk = remaining;
                if ((unsigned int)remaining >= READ_CHUNK_SIZE)
                    chunk = READ_CHUNK_SIZE;
                if (ReadFile(file, read_buffer, chunk, &bytes_read, 0) && bytes_read == (unsigned int)chunk)
                    crc_checksum_buffer(&crc_reference, read_buffer, chunk);
                sound_idle();
                remaining -= chunk;
            }

            if (crc_reference == stored_checksum)
            {
                CloseHandle(file);
                return 1;
            }
            if (corrupted && stored_checksum)
            {
                *corrupted = 1;
                CloseHandle(file);
                return 0;
            }
            CloseHandle(file);
            return 0;
        }
    }

    /* file unreadable or wrong size: delete the corrupt profile file */
    char full_path[256];
    if (player_ui_get_path_to_local_player_profile_directory(0, full_path))
        DeleteFileA(full_path);
    CloseHandle(file);
    return 0;
}
