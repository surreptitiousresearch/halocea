#pragma once
/* oddball_statistics — oddball arm of the multiplayer_statistics union (6 bytes, DB types_members-confirmed). */

typedef struct oddball_statistics
{
    __int16 time_with_the_ball;    /* 0x00 */
    __int16 ball_carrier_kills;    /* 0x02 */
    __int16 kills_as_ball_carrier; /* 0x04 */
} oddball_statistics; /* 6 bytes */
