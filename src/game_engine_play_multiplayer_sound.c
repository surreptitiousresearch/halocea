#include <stdint.h>
/* game_engine_play_multiplayer_sound @0x837B6D9C — play a multiplayer game-engine sound for all
 * players (player index -1). */

extern void game_engine_play_multiplayer_sound_to_specific_player(int player_index, int index, uint8_t should_replicate);

void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate)
{
    game_engine_play_multiplayer_sound_to_specific_player(-1, index, should_replicate);
}
