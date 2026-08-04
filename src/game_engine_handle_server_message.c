/* game_engine_handle_server_message @0x837463D8 — dispatches an encoded server message to the active game
 * engine's handle_server_message callback, if any. */

#include <stdint.h>
#include "headers/game_engine.h"


void game_engine_handle_server_message(void *encoded_message, int16_t encoded_message_size)
{
    if (game_engine && game_engine->handle_server_message)
        game_engine->handle_server_message(encoded_message, encoded_message_size);
}
