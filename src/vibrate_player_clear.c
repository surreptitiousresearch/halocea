/* vibrate_player_clear @0x837392E0 — zero out a local player's controller-vibration state. */

#include <stdint.h>
#include <string.h>
#include "headers/vibrate_global_data.h"

void vibrate_player_clear(int16_t local_player_index)
{
    memset(&vibrate_globals->player_vibrate_data[local_player_index], 0,
           sizeof(vibrate_globals->player_vibrate_data[local_player_index]));
}
