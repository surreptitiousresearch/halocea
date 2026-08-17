/* slayer_engine_game_starting @0x83815458 */
#include <stdint.h>
#include "headers/game_engine_multiplayer_sound.h"

extern uint8_t game_engine_has_teams(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);

void slayer_engine_game_starting(void)
{
    game_engine_play_multiplayer_sound(
        game_engine_has_teams() ? _multiplayer_sound_team_slayer : _multiplayer_sound_slayer, 0);
}
