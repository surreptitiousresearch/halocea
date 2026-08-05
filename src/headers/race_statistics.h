#pragma once
#include <stdint.h>
/* race_statistics — race arm of the multiplayer_statistics union (6 bytes, DB types_members-confirmed). */

typedef struct race_statistics
{
    int16_t last_lap_time; /* 0x00 */
    int16_t laps;          /* 0x02 */
    int16_t best_lap_time; /* 0x04 */
} race_statistics; /* 6 bytes */
