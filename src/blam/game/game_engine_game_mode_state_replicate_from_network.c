/* game_engine_game_mode_state_replicate_from_network @0x8374A518 */
#include "headers/game_engine.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"

void game_engine_game_mode_state_replicate_from_network(message_delta_processor_header *header, network_game_client *client)
{
    void (*fn)(message_delta_processor_header *, network_game_client *) = game_engine->replicate_game_mode_state_from_network;
    if (fn)
        fn(header, client);
}
