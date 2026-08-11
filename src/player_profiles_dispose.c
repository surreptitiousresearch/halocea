/* player_profiles_dispose @0x83764D90 — shut down the async player-profile subsystem: wait for its
 * worker thread to exit, dispose it, and clear all globals.
 *
 * The trailing memset residue lands in r3 but no caller consumes it (saved_game_files_dispose ignores
 * the return), so the attested return type is void. */

#include <stdint.h>
#include "headers/player_profile_globals.h"
#include <string.h>

extern uint8_t thread_has_exited(const thread *thread_reference);
extern void dispose_thread(thread *thread_reference);

void player_profiles_dispose(void)
{
    if ( player_profile_globals.thread )
    {
        while ( !thread_has_exited(player_profile_globals.thread) )
            ;
        dispose_thread(player_profile_globals.thread);
        player_profile_globals.thread = 0;
    }
    memset(&player_profile_globals, 0, sizeof(player_profile_globals));
}
