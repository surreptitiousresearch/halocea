/* do_write_profile_block @0x837657C8 — write one enumerated saved-game profile slot: copy the 0x7BC-byte
 * profile into a local block, append its checksum (making 0x7C0 bytes), rewind and write the file, and on
 * a clean close sync the enumerated metadata's display name with the profile's player name. If positioning
 * or writing failed, the (corrupt) enumerated file is deleted. */

#include <stdint.h>
#include "headers/file_reference.h"
#include "headers/player_profile.h"
#include <string.h>

extern uint8_t saved_game_file_open(file_reference *saved_game_file, int index);
extern uint8_t saved_game_file_close(file_reference *saved_game_file, int index);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);
extern uint8_t file_set_position(const file_reference *file, unsigned int position);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern int synchronize_metadata_display_name_with_profile_name(int index, uint16_t *game_display_name);
extern int delete_enumerated_saved_game_file(int index);

void do_write_profile_block(int index, player_profile *profile)
{
    uint8_t write_failed = 0;
    file_reference file;

    if ( saved_game_file_open(&file, index) )
    {
        struct
        {
            unsigned char data[0x7BC];
            unsigned char checksum[4];
        } block;
        memcpy(block.data, profile, sizeof(block.data));
        saved_game_file_generate_checksum(block.data, 0x7BCu, block.checksum);

        if ( !file_set_position(&file, 0) || !file_write(&file, 0x7C0u, &block) )
            write_failed = 1;

        if ( saved_game_file_close(&file, index) )
            synchronize_metadata_display_name_with_profile_name(index, profile->player_name);
    }

    if ( write_failed )
        delete_enumerated_saved_game_file(index);
}
