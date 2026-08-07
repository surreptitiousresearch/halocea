/* playlist_profile_read @0x837823A0 — load a game variant from a saved-game slot. Sibling of
 * player_profile_read.c, same shape: first join any pending profile-IO thread. For a default index (>= 0)
 * build a default "Slayer" variant stamped with the slot's display name. For a real saved file (index < 0)
 * take the saved-game mutex, read the 0x200 record, verify its stored checksum (covering the leading
 * 152-byte game_variant body); on mismatch fall back to the same default-variant construction, otherwise
 * use the record as-is. Returns 1 on success.
 *
 * DEVIATION: the record is read as one 512-byte buffer but the checksum only covers the first 152 bytes
 * (sizeof(game_variant)) with its 4-byte stored checksum immediately following — modeled as a single
 * buffer to match the stack layout, as in player_profile_read.c. The checksum compare is the decompiler's
 * byte loop, reproduced verbatim (sets a nonzero difference on the first mismatching byte). */

#include <stdint.h>
#include <string.h>
#include "headers/game_variant.h"
#include "headers/playlist_profile_globals.h"
#include "headers/file_reference.h"

extern uint8_t thread_has_exited(const thread *thread_reference);
extern void dispose_thread(thread *thread_reference);
extern game_variant * build_game_variant_slayer(game_variant *result);
extern uint16_t *saved_game_file_get_display_name(int index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern uint8_t saved_game_files_take_mutex(void);
extern void saved_game_files_release_mutex(void);
extern uint8_t saved_game_file_open(file_reference *saved_game_file, int index);
extern uint8_t saved_game_file_close(file_reference *saved_game_file, int index);
extern uint8_t file_read(const file_reference *file, unsigned int count, void *buffer);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);

int playlist_profile_read(int index, game_variant *variant)
{
    int succeeded = 0;
    file_reference reference;
    game_variant default_variant;

    thread *io_thread = playlist_profile_globals.thread;
    if ( io_thread )
    {
        do
            io_thread = playlist_profile_globals.thread;
        while ( !thread_has_exited(io_thread) );
        dispose_thread(playlist_profile_globals.thread);
        playlist_profile_globals.thread = 0;
    }

    if ( index >= 0 )
    {
        build_game_variant_slayer(&default_variant);
        default_variant.flags = 0;
        ustrncpy(default_variant.human_readable_game_description, saved_game_file_get_display_name(index), 0x17u);
        default_variant.human_readable_game_description[23] = 0;
        memcpy(variant, &default_variant, sizeof(game_variant));
        return 1;
    }

    if ( saved_game_files_take_mutex() )
    {
        int result = saved_game_file_open(&reference, index);
        if ( (unsigned char)result )
        {
            unsigned char record[512];   /* 152-byte game_variant body + 4-byte stored checksum */
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

                game_variant *source;
                if ( difference )
                {
                    build_game_variant_slayer(&default_variant);
                    default_variant.flags = 0;
                    ustrncpy(default_variant.human_readable_game_description,
                            saved_game_file_get_display_name(index), 0x17u);
                    default_variant.human_readable_game_description[23] = 0;
                    source = &default_variant;
                }
                else
                {
                    source = (game_variant *)record;
                }
                memcpy(variant, source, sizeof(game_variant));
                succeeded = 1;
            }
            result = saved_game_file_close(&reference, index);
        }
        saved_game_files_release_mutex();
    }

    return succeeded;
}
