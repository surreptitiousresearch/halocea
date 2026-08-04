/* player_profile_write @0x83765890 — persist a player profile to its saved-game file: open the file, copy the
 * 1980-byte profile body into a scratch buffer, append a checksum, write the 0x7C0-byte record at offset 0,
 * and close. On open/write failure the enumerated saved-game file is deleted. On successful close the saved
 * file's metadata display name is synchronized with the profile name.
 *
 * Deviation: the original sets a failure flag when file_set_position OR file_write fails; reproduced as the
 * same short-circuit. The trailing `return 1` is unconditional (decompiler), reproduced verbatim. */

#include <stdint.h>
#include "headers/player_profile.h"
#include "headers/file_reference.h"

extern void *memcpy(void *dst, const void *src, unsigned int size);
extern uint8_t saved_game_file_open(file_reference *saved_game_file, int index);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);
extern uint8_t file_set_position(const file_reference *file, unsigned int position);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern uint8_t saved_game_file_close(file_reference *saved_game_file, int index);
extern int synchronize_metadata_display_name_with_profile_name(int index, uint16_t *game_display_name);
extern int delete_enumerated_saved_game_file(int index);

int player_profile_write(int index, player_profile *profile)
{
    char failed = 0;
    file_reference reference;
    /* 1980-byte profile body followed by the 4-byte checksum block: the engine writes the combined
     * 0x7C0 record, so the checksum slot is kept contiguous with the body (matches the stack layout). */
    unsigned char record[1984];

    if ( saved_game_file_open(&reference, index) )
    {
        memcpy(record, profile, sizeof(*profile));
        saved_game_file_generate_checksum(record, 0x7BCu, &record[1980]);
        if ( !file_set_position(&reference, 0) || !file_write(&reference, 0x7C0u, record) )
            failed = 1;
        if ( saved_game_file_close(&reference, index) )
            synchronize_metadata_display_name_with_profile_name(index, profile->player_name);
    }

    if ( failed )
        delete_enumerated_saved_game_file(index);
    return 1;
}
