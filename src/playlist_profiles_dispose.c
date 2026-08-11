/* playlist_profiles_dispose @0x83781EE0 — shut down the async playlist-profile subsystem: wait for its
 * worker thread to exit, dispose it, and clear all globals.
 *
 * The trailing memset residue lands in r3 but no caller consumes it (saved_game_files_dispose ignores
 * the return), so the attested return type is void. */

#include <stdint.h>
#include "headers/playlist_profile_globals.h"
#include <string.h>

extern uint8_t thread_has_exited(const thread *thread_reference);
extern void dispose_thread(thread *thread_reference);

void playlist_profiles_dispose(void)
{
    if ( playlist_profile_globals.thread )
    {
        while ( !thread_has_exited(playlist_profile_globals.thread) )
            ;
        dispose_thread(playlist_profile_globals.thread);
        playlist_profile_globals.thread = 0;
    }
    memset(&playlist_profile_globals, 0, sizeof(playlist_profile_globals));
}
