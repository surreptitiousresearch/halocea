#include "headers/blam_data_globals.h"
/* game_engine_player_score_remove_player @0x8374FF84 — free a player's score-table slot: mark it
 * unused and decrement the used-slot count. Slots are stride 48; is_slot_used is the slot's first byte. */

extern int game_engine_player_score_get_slot_index_for_player(int player_index);

void game_engine_player_score_remove_player(int player_index)
{
    int slot_index = game_engine_player_score_get_slot_index_for_player(player_index);
    g_player_score_baselines[slot_index].is_slot_used = 0;   /* player_score_baseline_slot.is_slot_used @ +0 */
    g_player_score_number_of_used_slots = g_player_score_number_of_used_slots - 1;
}
