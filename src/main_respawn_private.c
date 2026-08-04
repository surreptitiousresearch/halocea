/* main_respawn_private @0x8368A530 — after the respawn request has been pending >90 unpaused, non-cinematic
 * ticks, respawn the co-op players and clear the request once they have respawned.
 *
 * Deviation: database types this int; all values are ABI residue from the predicate calls. Written as void. */

#include <stdint.h>
#include "headers/main_globals.h"

extern uint8_t game_time_get_paused(void);
extern uint8_t cinematic_in_progress(void);
extern int game_time_get(void);
extern void hcex_output_dbg(const char *format, ...);
extern uint8_t players_respawn_coop(void);

void main_respawn_private(void)
{
    if ( !game_time_get_paused() && !cinematic_in_progress() && main_globals.respawn_timer++ > 90 )
    {
        hcex_output_dbg("main_respawn_private tick %d\n", game_time_get());
        if ( players_respawn_coop() )
        {
            main_globals.respawn = 0;
            main_globals.respawn_timer = 0;
        }
    }
}
