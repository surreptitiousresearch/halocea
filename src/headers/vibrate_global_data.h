#pragma once
/* vibrate_global_data — controller rumble state. `vibrate_globals` accumulates per-frame rumble from up
 * to 8 concurrent impulse sources (each with two frequency channels) plus a scripted override, for each
 * of the 2 local controllers, then drives the gamepad motors. Layout from the database. */

#include "player_vibrate_datum.h"
#include "scripted_vibrate_datum.h"

typedef struct vibrate_global_data
{
    player_vibrate_datum   player_vibrate_data[2]; /* 0x000 */
    scripted_vibrate_datum scripted_vibrate;       /* 0x410 */
} vibrate_global_data;              /* 1052 bytes */

extern vibrate_global_data *vibrate_globals;
