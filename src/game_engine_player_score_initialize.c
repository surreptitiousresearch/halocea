/* game_engine_player_score_initialize @0x8374A428 — one-time init of the player score baseline table:
 * zero every slot, then clear the (redundant) is_slot_used flag once more per slot, and reset the used-slot
 * count.
 *
 * DEVIATION: the decompiler surfaced memset's r3 as a `void *` return. r3 at blr is only ever the callee's,
 * and there are no callers that consume it — attested void.
 *
 * DEVIATION: the decompiler rendered the is_slot_used-clearing loop against
 * `structure_globals.cluster_magic_numbers[502]` — a coincidentally-adjacent symbol whose bounds don't fit
 * (32 iterations of a 48-byte stride from index 502 run past the 512-int array). disasm_range(0x8374A428,
 * 0x8374A484) shows the loop is addressed off g_player_score_baselines itself (`addi r11, r31, -0x30` then
 * `stbu r10, 0x30(r11)` x32, i.e. is_slot_used at the front of each of the 32 player_score_baseline_slot
 * entries), matching player_score_baseline_slot.h's layout exactly. */

#include <string.h>
#include "headers/player_score_baseline_slot.h"
#include "headers/blam_data_globals.h"


void game_engine_player_score_initialize(void)
{
    if ( !g_player_score_is_initialized )
    {
        memset(g_player_score_baselines, 0, sizeof(g_player_score_baselines));
        for ( int i = 0; i < 32; ++i )
            g_player_score_baselines[i].is_slot_used = 0;

        g_player_score_number_of_used_slots = 0;
        g_player_score_is_initialized = 1;
    }
}
