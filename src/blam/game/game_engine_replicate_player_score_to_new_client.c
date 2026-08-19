/* game_engine_replicate_player_score_to_new_client @0x83752C98 — replicate one player's score slot to a
 * newly-joined client in stateless mode.
 *
 * DEVIATION: disasm shows r5 (the callee's `machine_index` argument) is never assigned in this function —
 * it passes through whatever value was live in r5 at entry, and the callee (game_engine_replicate_
 * player_score_to_network) never reads its own `machine_index` parameter either, so the value is genuinely
 * dead end-to-end. The database's own arg-count analysis agrees (2 args), so no third parameter is added
 * here; 0 is passed explicitly in its place. */

#include "headers/message_delta_processor_mode.h"

extern int game_engine_player_score_get_slot_index_for_player(int player_index);
extern void game_engine_replicate_player_score_to_network(int slot, message_delta_processor_mode mode, const int machine_index);

void game_engine_replicate_player_score_to_new_client(int existing_player_index, int new_client_machine_index)
{
    int slot_index_for_player = game_engine_player_score_get_slot_index_for_player(existing_player_index);
    if ( slot_index_for_player != -1 )
        game_engine_replicate_player_score_to_network(slot_index_for_player, _message_delta_mode_stateless, 0);
}
