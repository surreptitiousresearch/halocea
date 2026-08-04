/* player_ui_clear_multiplayer_variant @0x836989BC — reset the selected multiplayer variant: clear the
 * UI selection, drop the game connection, tear down the game engine, and clear the active variant. */

#include <stdint.h>
#include "headers/player_ui_globals.h"

#include "headers/game_variant.h"
extern void game_connection_set(int16_t new_connection);
extern void game_engine_dispose(void);
extern void game_set_game_variant(game_variant *variant);

void player_ui_clear_multiplayer_variant(void)
{
    player_ui_globals.user_selected_variant = 0;
    game_connection_set(0);
    game_engine_dispose();
    game_set_game_variant(0);
}
