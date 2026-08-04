/* game_engine_player_damaged_player @0x837471C0 — forward a "player damaged player" notification to the
 * active game engine's variant handler, if one is installed. No-op in free play (no game engine) or when
 * the variant does not hook the event. The original is a tail call through game_engine->player_damaged_player. */

#include <stdint.h>
#include "headers/game_engine.h"


void game_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire)
{
    if ( game_engine && game_engine->player_damaged_player )
        game_engine->player_damaged_player(killing_player_index, dead_player_index, friendly_fire);
}
