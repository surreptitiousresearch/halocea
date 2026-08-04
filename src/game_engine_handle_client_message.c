/* game_engine_handle_client_message @0x837463A8 — dispatches an encoded client message to the active game
 * engine's handle_client_message callback, if any. */

#include <stdint.h>
#include "headers/game_engine.h"


void game_engine_handle_client_message(int player_index, void *encoded_message, int16_t encoded_message_size)
{
    if (game_engine && game_engine->handle_client_message)
        game_engine->handle_client_message(player_index, encoded_message, encoded_message_size);
}
