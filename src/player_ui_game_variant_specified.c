/* player_ui_game_variant_specified @0x83698B00 — if the user has selected a game variant in the UI,
 * copies it out and returns true; otherwise returns false. */

#include <stdint.h>
#include <string.h>
#include "headers/player_ui_globals.h"


uint8_t player_ui_game_variant_specified(game_variant *variant)
{
    if (player_ui_globals.user_selected_variant)
        memcpy(variant, &player_ui_globals.variant, sizeof(game_variant));
    return player_ui_globals.user_selected_variant;
}
