#pragma once
#include <stdint.h>
/* oddball_statistics — oddball arm of the multiplayer_statistics union (6 bytes, DB types_members-confirmed). */

typedef struct oddball_statistics
{
    int16_t time_with_the_ball;    /* 0x00 */
    int16_t ball_carrier_kills;    /* 0x02 */
    int16_t kills_as_ball_carrier; /* 0x04 */
} oddball_statistics; /* 6 bytes */
