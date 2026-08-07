/* playlist_profile_get_from_path @0x83782080 — load a game_variant from an absolute file path: open and
 * read a 512-byte record (a 152-byte game_variant body followed by its 4-byte stored checksum), verify the
 * checksum, and on success copy the variant out. Returns 1 on success, 0 on any failure (open/read/checksum
 * mismatch). */

#include <stdint.h>
#include <string.h>
#include "headers/game_variant.h"
#include "headers/file_reference.h"

extern file_reference *file_reference_create_from_path_absolute(file_reference *reference, const char *path, uint8_t directory);
extern uint8_t file_open(file_reference *file, unsigned int flags);
extern uint8_t file_read(const file_reference *file, unsigned int count, void *buffer);
extern uint8_t file_close(file_reference *file);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);

int playlist_profile_get_from_path(char *full_path, game_variant *variant)
{
    int succeeded = 0;
    file_reference reference;

    if ( file_reference_create_from_path_absolute(&reference, full_path, 0) && file_open(&reference, 1u) )
    {
        unsigned char record[516];   /* 152-byte game_variant body + 4-byte stored checksum */
        if ( file_read(&reference, 0x200u, record) )
        {
            unsigned char computed_checksum[4];
            saved_game_file_generate_checksum(record, 0x98u, computed_checksum);

            unsigned char *computed = computed_checksum;
            unsigned char *stored = &record[152];
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

            if ( !difference )
            {
                memcpy(variant, record, sizeof(game_variant));
                succeeded = 1;
            }
        }
        file_close(&reference);
    }
    return succeeded;
}
