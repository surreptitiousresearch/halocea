#pragma once
/* player_vibrate_datum — accumulated rumble state for one local controller: up to 8 concurrent
 * impulse sources plus continuous-frequency overrides. Layout from the database. */

#include "vibrate_definition.h"

typedef struct player_vibrate_datum
{
    vibrate_definition impulse[8];                 /* 0x000 */
    float              time_elapsed[8];            /* 0x1E0 — per-impulse elapsed seconds */
    float              left_frequency_continuous;  /* 0x200 */
    float              right_frequency_continuous; /* 0x204 */
} player_vibrate_datum;             /* 520 bytes */
