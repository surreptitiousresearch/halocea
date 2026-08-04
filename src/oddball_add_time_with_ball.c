/* oddball_add_time_with_ball @0x838162E8 — advances a carrying player's oddball score by one tick;
 * posts a HUD status message (40) first if playing the "normal" oddball variant (time accumulation is
 * silent for the other ball types). */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/oddball_message.h"

extern game_variant *game_engine_get_variant(void);
extern void game_engine_state_message(int player_index, int message, int message_data);
extern int16_t game_connection(void);
extern void oddball_add_score(int player_index);

void oddball_add_time_with_ball(int player_index)
{
    if (game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type == _oddball_normal)
        game_engine_state_message(player_index, oddball_message_you_have_the_ball_tick, player_index);

    game_connection();
    oddball_add_score(player_index);
}
