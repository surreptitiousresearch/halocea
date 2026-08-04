/* game_state_revert @0x83684A80 — revert to the last saved game state. If there is no valid saved game (and
 * the recovery hack is off) just reset the map; otherwise run the before-load procs, read the saved state from
 * file, and run the after-load procs. */

#include "headers/game_state_globals.h"
#include "headers/blam_data_globals.h"

extern void main_reset_map(void);
extern void game_state_call_before_load_procs(void);
extern void game_state_read_from_file(void);
extern void game_state_call_after_load_procs(void);   /* definition is void(void) — see below */

void game_state_revert(void) /* attested void: r3 at blr is callee residue, 0/3 callers consume */
{
    if ( !game_state_globals.saved_game_valid && !recover_saved_games_hack )
    {
        main_reset_map();
        return;
    }

    game_state_call_before_load_procs();
    game_state_read_from_file(); /* attested void: old int-return model was r3-threading */
    game_state_call_after_load_procs();
}
