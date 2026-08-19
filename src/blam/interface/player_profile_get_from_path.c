/* player_profile_get_from_path @0x83764E80 — load a player_profile from an absolute file path: open and
 * read a 0x7C0-byte record (a 1980-byte body followed by its 4-byte stored checksum, per
 * player_profile_read.c), verify the checksum and the version byte (record[0] == 9), and on success copy
 * the profile out. Returns 1 on success, 0 on any failure. */

#include <stdint.h>
#include <string.h>
#include "headers/player_profile.h"
#include "headers/file_reference.h"

extern file_reference *file_reference_create_from_path_absolute(file_reference *reference, const char *path, uint8_t directory);
extern uint8_t file_open(file_reference *file, unsigned int flags);
extern uint8_t file_read(const file_reference *file, unsigned int count, void *buffer);
extern uint8_t file_close(file_reference *file);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);

int player_profile_get_from_path(char *full_path, player_profile *profile)
{
    int succeeded = 0;
    file_reference reference;

    if ( file_reference_create_from_path_absolute(&reference, full_path, 0) && file_open(&reference, 1u) )
    {
        unsigned char record[1984];   /* 1980-byte body + 4-byte stored checksum */
        if ( file_read(&reference, 0x7C0u, record) )
        {
            unsigned char computed_checksum[4];
            saved_game_file_generate_checksum(record, 0x7BCu, computed_checksum);

            unsigned char *computed = computed_checksum;
            unsigned char *stored = &record[1980];
            int difference = 0;
            do
            {
                difference = *computed - *stored;
                if ( *computed != *stored )
                    break;
                ++computed;
                ++stored;
            }
            while ( computed != &computed_checksum[4] );

            if ( !difference && record[0] == 9 )
            {
                memcpy(profile, record, sizeof(player_profile));
                succeeded = 1;
            }
        }
        file_close(&reference);
    }
    return succeeded;
}
