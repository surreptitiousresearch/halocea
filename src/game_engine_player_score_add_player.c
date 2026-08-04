/* game_engine_player_score_add_player @0x8374FEE0 (real prologue — mflr/stwu/`mr r6,r3`; the funcs-table
 * address 0x8374FEF8 is mid-prologue) — allocates the player's score baseline slot if it doesn't already
 * have one, scanning the fixed 8-slot table (see game_engine_player_score_remove_player.c) for a free entry.
 *
 * DEVIATION: the decompiler's loop bound `(int)v3 >= (int)global_goal` is a coincidental adjacent-global
 * address collision (same class of artifact as other tiers' fake-offsetof casts) — the table is a fixed
 * 384-byte / 8-slot array, so the real bound is simply "8 slots", not a read of global_goal. Also DEVIATION:
 * the decompiler's `v1` (stored into player_index) reads as uninitialized in its own view;
 * disasm_range(0x8374FEC0,0x8374FF00) confirms it's `player_index` itself, copied into r6 to survive the
 * game_engine_player_score_get_slot_index_for_player call. */

#include "headers/player_score_baseline_slot.h"
#include "headers/blam_data_globals.h"

extern int game_engine_player_score_get_slot_index_for_player(int player_index);

void game_engine_player_score_add_player(int player_index)
{
    if ( game_engine_player_score_get_slot_index_for_player(player_index) == -1 )
    {
        int slot_index = 0;
        while ( g_player_score_baselines[slot_index].is_slot_used )
        {
            ++slot_index;
            if ( slot_index >= 8 )
                return;
        }

        ++g_player_score_number_of_used_slots;
        g_player_score_baselines[slot_index].is_slot_used = 1;
        g_player_score_baselines[slot_index].player_index = player_index;
    }
}
