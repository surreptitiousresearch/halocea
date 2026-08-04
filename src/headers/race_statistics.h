#pragma once
/* race_statistics — race arm of the multiplayer_statistics union (6 bytes, DB types_members-confirmed). */

typedef struct race_statistics
{
    __int16 last_lap_time; /* 0x00 */
    __int16 laps;          /* 0x02 */
    __int16 best_lap_time; /* 0x04 */
} race_statistics; /* 6 bytes */
