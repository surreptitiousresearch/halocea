/* player_ui_dispose @ 0x83698818 — zero the player-UI globals */

#include <string.h>
#include "headers/player_ui_globals.h"

/* void: tail-call to memset, but the sole caller (game_dispose) ignores r3. */
void player_ui_dispose(void)
{
    memset(&player_ui_globals, 0, sizeof(player_ui_globals));
}
