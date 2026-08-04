/* multiplayer_message_internal @0x8374D560 — build a localized multiplayer message string for a player.
 * Gives the active game engine's rasterize_score hook first chance to produce it; if that declines (returns
 * 0), falls back to the built-in internal_rasterize_score. Returns nonzero if a message was produced.
 * DEVIATION: the decompiler cast the hook to a 1-arg call; the disassembly leaves message/message_data/
 * buffer/buffer_size in r4-r7, so the hook receives all five arguments. */

#include <stdint.h>
#include "headers/game_engine.h"

extern int internal_rasterize_score(int player_index, int message, int message_data, uint16_t *buffer, unsigned int buffer_size);

uint8_t multiplayer_message_internal(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size)
{
    int result = 0;
    if ( game_engine->rasterize_score )
        result = game_engine->rasterize_score(player_index, message, message_data, buffer, buffer_size);
    if ( !(unsigned char)result )
        return internal_rasterize_score(player_index, message, message_data, buffer, buffer_size);
    return result;
}
