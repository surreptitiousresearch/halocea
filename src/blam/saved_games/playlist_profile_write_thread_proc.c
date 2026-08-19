/* playlist_profile_write_thread_proc @0x83782548 — background-thread entry point that writes a playlist
 * game_variant profile to disk: `input` is [index:int][game_variant:152 bytes] packed contiguously by the
 * caller. Copies the variant into a 512-byte on-disk record (152-byte body + 4-byte checksum, matching
 * playlist_profile_get_from_path's on-disk layout — the remaining bytes are unspecified padding, as
 * compiled), writes it, resyncs the saved-game metadata display name from the variant's own description
 * string, and deletes the enumerated file on any failure. Always returns 0. */

#include <stdint.h>
#include <string.h>
#include "headers/file_reference.h"

extern uint8_t saved_game_files_take_mutex(void);
extern void saved_game_files_release_mutex(void);
extern uint8_t saved_game_file_open(file_reference *saved_game_file, int index);
extern uint8_t saved_game_file_close(file_reference *saved_game_file, int index);
extern uint8_t file_set_position(const file_reference *file, unsigned int position);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);
extern int synchronize_metadata_display_name_with_profile_name(int index, uint16_t *game_display_name);
extern int delete_enumerated_saved_game_file(int index);

unsigned int playlist_profile_write_thread_proc(int *input)
{
    if ( saved_game_files_take_mutex() )
    {
        int index = *input;
        uint8_t failed = 0;
        char *variant_data = (char *)(input + 1);

        file_reference reference;
        uint8_t result = saved_game_file_open(&reference, index);

        if ( result )
        {
            unsigned char record[512];   /* 152-byte game_variant body + 4-byte checksum + padding */
            memcpy(record, variant_data, 152);
            saved_game_file_generate_checksum(record, 0x98u, &record[152]);

            if ( !file_set_position(&reference, 0) || !file_write(&reference, 0x200u, record) )
                failed = 1;

            result = saved_game_file_close(&reference, index);
            if ( result )
                result = synchronize_metadata_display_name_with_profile_name(index, (uint16_t *)variant_data);
        }

        if ( failed )
            result = delete_enumerated_saved_game_file(index);

        saved_game_files_release_mutex();
    }
    return 0;
}
