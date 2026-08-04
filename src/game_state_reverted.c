/* game_state_reverted @0x83684C48 — return true if the game state was just reverted (reloaded) to a
 * checkpoint this frame, unless cinematic skipping is being held off. */

#include <stdint.h>
#include "headers/game_state_globals.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

uint8_t game_state_reverted(void)
{
    return !hcex_dont_skip_cine && game_time_get() == game_state_globals.revert_time;
}
