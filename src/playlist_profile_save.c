/* playlist_profile_save @0x83782758 — validated wrapper around the playlist_profile_write.c write path: no-op
 * if there's no profile selected (index -1); otherwise cleans up the variant, waits for any in-flight write
 * thread to finish, disposes it, stages the (index, variant) write request into thread_input
 * (see playlist_profile_write.c), and kicks off a fresh write thread. */

#include <stdint.h>
#include "headers/playlist_profile_globals.h"
#include "headers/game_variant.h"
#include <string.h>

extern void game_engine_variant_cleanup(game_variant *variant);
extern uint8_t thread_has_exited(const thread *thread_reference);
extern void dispose_thread(thread *thread_reference);
extern unsigned int playlist_profile_write_thread_proc(int *input);
extern uint8_t create_thread(uint16_t flags, unsigned int (*function)(void *), void *function_input, thread **thread_reference);

void playlist_profile_save(int playlist_profile_index, game_variant *variant)
{
    if ( playlist_profile_index == -1 )
        return;

    game_engine_variant_cleanup(variant);

    thread *write_thread = playlist_profile_globals.thread;
    if ( write_thread )
    {
        uint8_t not_yet_exited;
        do
        {
            not_yet_exited = thread_has_exited(write_thread) == 0;
            write_thread = playlist_profile_globals.thread;
        }
        while ( not_yet_exited );

        dispose_thread(playlist_profile_globals.thread);
        playlist_profile_globals.thread = nullptr;
    }

    playlist_profile_globals.thread_input.index = playlist_profile_index;
    memcpy(&playlist_profile_globals.thread_input.variant, variant, sizeof(game_variant));

    create_thread(0, playlist_profile_write_thread_proc, &playlist_profile_globals, &playlist_profile_globals.thread);
}
