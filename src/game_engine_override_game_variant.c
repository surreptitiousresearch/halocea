/* game_engine_override_game_variant @0x83749698 */
#include "headers/play_stage.h"
#include <string.h>

void game_engine_override_game_variant(game_variant *variant)
{
    if (variant)
        memcpy(&global_stage.game_variant, variant, sizeof(game_variant));
}
