/* game_time_set_paused @ 0x8369A918 — pause/resume the game clock. While initialized, the
 * clock is active exactly when not paused. */

#include <stdint.h>
#include "headers/game_time_globals.h"

void game_time_set_paused(uint8_t paused)   /* stb r3,2(r11) => 8-bit param */
{
    int was_initialized = game_time_globals->initialized;
    game_time_globals->paused = paused;
    if ( was_initialized )
        game_time_globals->active = !paused;   /* (_cntlzw(paused) & 0x20) != 0 */
}
