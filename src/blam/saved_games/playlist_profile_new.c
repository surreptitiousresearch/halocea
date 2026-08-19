/* playlist_profile_new @0x83781F48 — create a new playlist profile save file: enumerates a fresh save
 * slot, builds a default (slayer) game variant as the profile's starting content, clears its "in-progress
 * game" flag bit (flags & ~1), runs the standard variant cleanup pass, stamps the given display name into
 * the variant's description field, appends a checksum, and writes the whole 512-byte record to disk. On
 * any failure, deletes the enumerated slot and returns -1; otherwise returns its index. */

#include <stdint.h>
#include <string.h>
#include "headers/game_variant.h"
#include "headers/game_variant_flags.h"
#include "headers/file_reference.h"
#include "headers/saved_game_file_type.h"

extern int create_enumerated_saved_game_file(uint16_t saved_game_file_type, int16_t local_player_index, const uint16_t *display_name);
extern uint8_t saved_game_file_open(file_reference *saved_game_file, int index);
extern uint8_t saved_game_file_close(file_reference *saved_game_file, int index);
extern int delete_enumerated_saved_game_file(int index);
extern game_variant * build_game_variant_slayer(game_variant *result);
extern void game_engine_variant_cleanup(game_variant *variant);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);
extern uint8_t file_set_position(const file_reference *file, unsigned int position);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
/* memcpy declared by <string.h> */

int playlist_profile_new(int16_t local_player_index, uint16_t *name)
{
    int saved_game_index = create_enumerated_saved_game_file(_saved_game_file_type_play_list, local_player_index, name);
    if ( saved_game_index == -1 )
        return saved_game_index;

    file_reference saved_game_file;
    if ( !saved_game_file_open(&saved_game_file, saved_game_index) )
    {
        delete_enumerated_saved_game_file(saved_game_index);
        return -1;
    }

    unsigned char record[512];
    memset(record, 0, sizeof(record));

    game_variant variant;
    game_variant *default_variant = build_game_variant_slayer(&variant);
    memcpy(record, default_variant, sizeof(game_variant));
    ((game_variant *)record)->flags = default_variant->flags & (uint16_t)~(1u << _variant_is_system_default_bit);

    game_engine_variant_cleanup((game_variant *)record);
    ustrncpy((uint16_t *)record, name, 0x17u);
    ((game_variant *)record)->human_readable_game_description[23] = 0;  /* terminate the 24-wchar name */

    saved_game_file_generate_checksum(record, sizeof(game_variant), &record[152]);

    if ( !file_set_position(&saved_game_file, 0) || !file_write(&saved_game_file, sizeof(record), record) )
    {
        delete_enumerated_saved_game_file(saved_game_index);
        saved_game_index = -1;
    }

    saved_game_file_close(&saved_game_file, saved_game_index);
    return saved_game_index;
}
