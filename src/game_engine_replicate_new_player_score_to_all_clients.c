/* game_engine_replicate_new_player_score_to_all_clients @0x83752CC8 — broadcast a newly-added player's
 * score slot to all clients in stateless mode. Same dead-passthrough machine_index as
 * game_engine_replicate_player_score_to_new_client (see its DEVIATION note); 0 stands in for it here too. */

#include "headers/message_delta_processor_mode.h"

extern int game_engine_player_score_get_slot_index_for_player(int player_index);
extern void game_engine_replicate_player_score_to_network(int slot, message_delta_processor_mode mode, const int machine_index);

void game_engine_replicate_new_player_score_to_all_clients(int new_player_index)
{
    int slot_index_for_player = game_engine_player_score_get_slot_index_for_player(new_player_index);
    if ( slot_index_for_player != -1 )
        game_engine_replicate_player_score_to_network(slot_index_for_player, _message_delta_mode_stateless, 0);
}
