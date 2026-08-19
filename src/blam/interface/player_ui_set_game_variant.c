/* player_ui_set_game_variant @0x83698AB8 — copy a game variant into the player-UI globals and flag it as
 * user-selected. */

#include <string.h>
#include "headers/player_ui_globals.h"
#include "headers/game_variant.h"


void player_ui_set_game_variant(game_variant *variant)
{
    memcpy(&player_ui_globals.variant, variant, sizeof(player_ui_globals.variant));
    player_ui_globals.user_selected_variant = 1;
}
