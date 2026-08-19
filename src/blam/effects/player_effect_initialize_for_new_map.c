/* player_effect_initialize_for_new_map @0x83737518 — zero all player-effect state, disable any active screen
 * fade, and stamp the reference time for effect timing. */

#include <string.h>
#include "headers/player_effect_globals.h"

extern int game_time_get(void);

void player_effect_initialize_for_new_map(void)
{
    player_effect_globals_definition *globals = player_effect_globals;

    memset(globals, 0, sizeof(player_effect_globals_definition));
    globals->screen_fade.ticks = -1;
    player_effect_globals->reference_time = game_time_get();
}
