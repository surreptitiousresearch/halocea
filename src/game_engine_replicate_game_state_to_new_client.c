/* game_engine_replicate_game_state_to_new_client @0x8374A4E8 — sends the current game mode state to a newly
 * connected client in stateless mode. */

#include "headers/game_engine.h"

extern int _message_delta_mode_stateless;

void game_engine_replicate_game_state_to_new_client(const int machine_index)
{
    if (game_engine->replicate_game_mode_state_to_network)
        game_engine->replicate_game_mode_state_to_network((message_delta_processor_mode)_message_delta_mode_stateless, machine_index);
}
