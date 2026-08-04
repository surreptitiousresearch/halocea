/* player_profile_new @0x837659E8 — create a brand-new saved player profile named `name`: enumerate a
 * fresh saved-game file slot, build a default profile into it (flags reset to 0, name copied and
 * null-terminated), checksum and write the 0x7C0-byte record (1980-byte body + 4-byte checksum, matching
 * player_profile_read.c's on-disk layout), and clean up (deleting the enumerated file) on any failure.
 * Returns the new profile index, or -1. */

#include <stdint.h>
#include "headers/player_profile.h"
#include "headers/file_reference.h"
#include "headers/saved_game_file_type.h"

extern void *memset(void *destination, int value, unsigned int size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

extern int create_enumerated_saved_game_file(uint16_t saved_game_file_type, int16_t local_player_index, const uint16_t *display_name);
extern uint8_t saved_game_file_open(file_reference *saved_game_file, int index);
extern uint8_t saved_game_file_close(file_reference *saved_game_file, int index);
extern void player_profile_build_default_profile(player_profile *profile, int player_index, uint8_t try_inherit);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);
extern uint8_t file_set_position(const file_reference *file, unsigned int position);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern int delete_enumerated_saved_game_file(int index);

/* name const-qualified 2026-07-30: read-only (ustrncpy src), builtin-profiles caller passes const data (C4090) */
int player_profile_new(int16_t local_player_index, const uint16_t *name)
{
    int profile_index = create_enumerated_saved_game_file(
        _saved_game_file_type_player_profile, local_player_index, name);
    if ( profile_index == -1 )
        return profile_index;

    file_reference reference;
    if ( !saved_game_file_open(&reference, profile_index) )
    {
        delete_enumerated_saved_game_file(profile_index);
        return -1;
    }

    unsigned char record[1984];   /* 1980-byte body + 4-byte stored checksum */
    memset(record, 0, sizeof(record));

    player_profile *profile = (player_profile *)record;
    player_profile_build_default_profile(profile, 0, 1u);
    profile->flags = 0;
    ustrncpy(profile->player_name, name, 0xBu);
    profile->player_name[11] = 0;

    saved_game_file_generate_checksum(record, 0x7BCu, &record[1980]);

    if ( !file_set_position(&reference, 0) || !file_write(&reference, 0x7C0u, record) )
    {
        delete_enumerated_saved_game_file(profile_index);
        profile_index = -1;
    }

    saved_game_file_close(&reference, profile_index);
    return profile_index;
}
