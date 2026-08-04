/* game_state_set_revert_time @ 0x83684DA0 — record the current game time as the
 * revert (checkpoint-restore) timestamp and unpause the game clock. */

#include <stdint.h>
#include "headers/game_state_globals.h"

extern int game_time_get(void);
extern void game_time_set_paused(uint8_t paused);

void game_state_set_revert_time(void)
{
    game_state_globals.revert_time = game_time_get();
    game_time_set_paused(0);
}
