/* main_lost_map_private @0x8368A480 — once the player has been "lost" for >90 unpaused ticks, recover by
 * resetting the map (single player, when hcex_reset_single_on_die is set) or reverting to the last
 * checkpoint.
 *
 * Deviation: the database types this as returning int, but every value is ABI residue from the predicate
 * calls; written as void. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_time_get_paused(void);
extern int16_t local_player_count(void);

void main_lost_map_private(void)
{
    if ( !game_time_get_paused() && main_globals.loss_timer++ > 90 )
    {
        main_globals.lost_map = 0;
        main_globals.loss_timer = 0;
        if ( hcex_reset_single_on_die && local_player_count() == 1 )
            main_globals.reset_map = 1;
        else
            main_globals.revert_map = 1;
        main_globals.switch_to_structure_bsp_index = -1;
        main_globals.save_map = 0;
        main_globals.lost_map = 0;
    }
}
