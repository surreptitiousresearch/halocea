/* game_engine_player_score_get_slot_index_for_player @0x8374A544 — find the score-table slot (0-31) of
 * a player, or -1 if none. Scans all 32 slots for one that is in use and owns the player index.
 *
 * DEVIATION: the binary unrolls the scan 4 slots per iteration; reproduced as a plain loop. Like the
 * binary, it returns the last match rather than breaking on the first. */

#include "headers/player_score_baseline_slot.h"
#include "headers/blam_data_globals.h"


int game_engine_player_score_get_slot_index_for_player(int player_index)
{
    int slot_index = -1;
    for ( int slot = 0; slot < 32; ++slot )
    {
        if ( g_player_score_baselines[slot].is_slot_used == 1 && g_player_score_baselines[slot].player_index == player_index )
            slot_index = slot;
    }
    return slot_index;
}
