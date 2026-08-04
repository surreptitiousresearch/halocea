/* game_engine_update_non_deterministic @ 0x8374D3C8 — drive the post-game state machine
 * (frame-rate dependent): count down the end-of-game delay, fade in the post-game options,
 * run the map-cycle countdown, and react to the continue/quit buttons. Server only for the
 * authoritative transitions. */
#include <stdint.h>
#include "headers/game_engine_globals.h"
#include "headers/game_connection.h"
#include "headers/control_button.h"
#include "headers/key_code.h"

#include "headers/network_game_server.h"
extern void *game_engine;
extern void vibrate_clear_all_now(void);
extern int16_t game_connection(void);
extern void game_engine_progress_to_postgame_rasterize(void);
extern void game_engine_change_mode_to_network(game_engine_mode new_mode);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t input_abstraction_is_action_button_down(int16_t local_player_index, int16_t button);
extern uint8_t input_key_is_down(int16_t key_code);
extern void network_game_abort(void); /* attested void */

void game_engine_update_non_deterministic(float seconds_elapsed)
{
    if ( !game_engine )
        return;

    if ( game_engine_globals.mode == game_engine_mode_postgame_rasterize_delay )
    {
        vibrate_clear_all_now();
        game_engine_globals.game_ending_timer -= seconds_elapsed;
        if ( game_engine_globals.game_ending_timer <= 0.0f && game_connection() == _game_connection_network_server )
        {
            game_engine_progress_to_postgame_rasterize();
            game_engine_change_mode_to_network(game_engine_mode_postgame_rasterize);
        }
    }
    else if ( game_engine_globals.mode == game_engine_mode_postgame_rasterize )
    {
        vibrate_clear_all_now();
        game_engine_globals.post_game_options_fade += seconds_elapsed;
        if ( game_engine_globals.post_game_options_fade > 1.0f )
            game_engine_globals.post_game_options_fade = 1.0f;

        if ( game_connection() == _game_connection_network_server )
        {
            char cycle_expired = 0;
            if ( game_engine_globals.map_cycle_countdown_enabled )
            {
                game_engine_globals.time_until_map_cycle_timeout_expires_seconds -= seconds_elapsed;
                if ( game_engine_globals.time_until_map_cycle_timeout_expires_seconds <= 0.0f )
                {
                    cycle_expired = 1;
                    game_engine_globals.map_cycle_countdown_enabled = 0;
                }
            }
            else
            {
                global_network_game_server_get();
            }
            if ( input_abstraction_is_action_button_down(0, _button_start) || input_key_is_down(_keypad_enter) == 1 || cycle_expired )
                global_network_game_server_get();
        }
        if ( input_abstraction_is_action_button_down(0, _button_back) )
            network_game_abort();
    }
}
