/* playlist_profile_write @0x837826D8 — wait for any in-flight write thread to finish, dispose it, stage a
 * new (index, variant) write request into thread_input (per disasm_range(0x837826D8,0x8378274C)),
 * and kick off a fresh write thread. */

#include <stdint.h>
#include "headers/playlist_profile_globals.h"
#include "headers/game_variant.h"
#include <string.h>

extern uint8_t thread_has_exited(const thread *thread_reference);
extern void dispose_thread(thread *thread_reference);
extern unsigned int playlist_profile_write_thread_proc(int *input);
extern unsigned __int8 create_thread(unsigned __int16 flags, unsigned int (__fastcall *function)(void *),
    void *function_input, thread **thread_reference);

unsigned __int8 playlist_profile_write(int index, game_variant *variant)
{
    while ( playlist_profile_globals.thread )
    {
        if ( !thread_has_exited(playlist_profile_globals.thread) )
            continue;
        dispose_thread(playlist_profile_globals.thread);
        playlist_profile_globals.thread = 0;
    }

    playlist_profile_globals.thread_input.index = index;
    memcpy(&playlist_profile_globals.thread_input.variant, variant, sizeof(game_variant));

    return create_thread(0, playlist_profile_write_thread_proc, &playlist_profile_globals,
        &playlist_profile_globals.thread);
}
