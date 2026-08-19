/* game_engine_end_game @0x8374D350 — on the server, end an active multiplayer game: enter the
 * postgame-delay phase, start the 7-second end-game timer, play the end sound, close UI, and replicate
 * the mode change to clients. */

#include <stdint.h>
#include "headers/game_engine_globals.h"
#include "headers/game_connection.h"
#include "headers/game_engine_multiplayer_sound.h"

typedef struct network_game_server network_game_server;
extern int16_t game_connection(void);
extern network_game_server *global_network_game_server_get(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void ui_widgets_close_all(void);
extern void game_engine_change_mode_to_network(game_engine_mode new_mode);

void game_engine_end_game(void)
{
    if ( game_connection() == _game_connection_network_server && game_engine_globals.mode == game_engine_mode_active )
    {
        global_network_game_server_get();
        game_engine_globals.mode = game_engine_mode_postgame_delay;
        game_engine_globals.game_ending_timer = 7.0f;
        game_engine_play_multiplayer_sound(_multiplayer_sound_game_over, 0);
        ui_widgets_close_all();
        game_engine_change_mode_to_network(game_engine_mode_postgame_delay);
    }
}
