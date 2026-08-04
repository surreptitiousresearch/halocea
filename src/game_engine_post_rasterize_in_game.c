/* game_engine_post_rasterize_in_game 0x83752ECC — per-frame in-game scoreboard overlay for the local player.
 * Draws the targeted-player name, then fades the score-display alpha in (while the score button is held or
 * during the post-game delay) or out, clamped to [0,1]. While the alpha is positive it renders the score with a
 * gamma curve (pow(alpha, 1.9)); the running alpha is stored back into game_engine_globals.score_alpha. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/control_button.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern int local_player_get_player_index(int16_t local_player_index);
extern void internal_rasterize_target_name(int player_index);
extern uint8_t input_abstraction_is_action_button_down(int16_t local_player_index, int16_t button);
extern void game_engine_rasterize_in_game_score(int player_index, float alpha);
extern double pow(double base, double exponent);

void game_engine_post_rasterize_in_game(void)
{
    __int16 local_player_index = render.local_player_index;
    int player_index = local_player_get_player_index(local_player_index);

    /* the original tests the player datum pointer for non-null alongside the game-engine pointer. */
    if ( game_engine && DATA_ARRAY_ELEMENT(player_data, player_datum, player_index) )
        internal_rasterize_target_name(player_index);

    float alpha = game_engine_globals.score_alpha[local_player_index];
    if ( input_abstraction_is_action_button_down(local_player_index, _button_showscores)
         || game_engine_globals.mode == game_engine_mode_postgame_delay )
        alpha = alpha + 0.06666667f;
    else
        alpha = alpha - 0.06666667f;

    if ( alpha < 0.0 )
    {
        game_engine_globals.score_alpha[local_player_index] = 0.0;
        return;
    }
    if ( alpha > 1.0 )
        alpha = 1.0;
    if ( alpha > 0.0 )
        game_engine_rasterize_in_game_score(player_index, (float)pow(alpha, 1.9f));
    game_engine_globals.score_alpha[local_player_index] = alpha;
}
