/* player_profile_read @0x83765620 — load a player profile from its saved-game file. First join any pending
 * profile-IO thread. For a default index (>= 0 here) build a default profile and stamp its display name. For
 * a real saved file (index < 0) take the saved-game mutex, open + read the 0x7C0 record, verify the stored
 * checksum and version byte (9); on mismatch fall back to a default profile carrying the file's display name,
 * otherwise use the record as-is. Returns 1 on success.
 *
 * Deviations:
 *   - The read record is a 1980-byte body followed by its 4-byte checksum; modeled as one contiguous 1984-byte
 *     buffer to match the stack layout (v20[1980] + v21).
 *   - The checksum compare is the decompiler's byte loop, reproduced verbatim (sets a nonzero difference on
 *     the first mismatching byte). */

#include <stdint.h>
#include "headers/player_profile.h"
#include "headers/player_profile_globals.h"
#include "headers/file_reference.h"

extern void *memcpy(void *dst, const void *src, unsigned int size);
extern uint8_t thread_has_exited(const thread *thread_reference);
extern void dispose_thread(thread *thread_reference);
extern void player_profile_build_default_profile(player_profile *profile, int player_index, uint8_t try_inherit);
extern uint16_t *saved_game_file_get_display_name(int index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern uint8_t saved_game_files_take_mutex(void);
extern void saved_game_files_release_mutex(void);
extern uint8_t saved_game_file_open(file_reference *saved_game_file, int index);
extern uint8_t saved_game_file_close(file_reference *saved_game_file, int index);
extern uint8_t file_read(const file_reference *file, unsigned int count, void *buffer);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);

uint8_t player_profile_read(int index, player_profile *profile)
{
    int succeeded = 0;
    file_reference reference;
    player_profile default_profile;

    thread *io_thread = player_profile_globals.thread;
    if ( io_thread )
    {
        do
            io_thread = player_profile_globals.thread;
        while ( !thread_has_exited(io_thread) );
        dispose_thread(player_profile_globals.thread);
        player_profile_globals.thread = 0;
    }

    if ( index >= 0 )
    {
        player_profile_build_default_profile(&default_profile, 0, 0);
        default_profile.flags = 0;
        ustrncpy(default_profile.player_name, saved_game_file_get_display_name(index), 0xBu);
        default_profile.player_name[11] = 0;
        memcpy(profile, &default_profile, sizeof(player_profile));
        return 1;
    }

    if ( saved_game_files_take_mutex() )
    {
        int result = saved_game_file_open(&reference, index);
        if ( (unsigned char)result )
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

                player_profile *source;
                if ( difference || record[0] != 9 )
                {
                    player_profile_build_default_profile(&default_profile, 0, 0);
                    default_profile.flags = 0;
                    ustrncpy(default_profile.player_name, saved_game_file_get_display_name(index), 0xBu);
                    default_profile.player_name[11] = 0;
                    source = &default_profile;
                }
                else
                {
                    source = (player_profile *)record;
                }
                memcpy(profile, source, sizeof(player_profile));
                succeeded = 1;
            }
            result = saved_game_file_close(&reference, index);
        }
        saved_game_files_release_mutex();
    }
    return succeeded;
}
