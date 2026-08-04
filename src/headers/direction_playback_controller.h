#pragma once
/* direction_playback_controller — accumulated yaw/pitch state driven by a sequence of
 * vector_char_difference_data ticks (e.g. cinematic/demo playback of recorded look input). */

typedef struct direction_playback_controller
{
    __int16 yaw;   /* 0x00 */
    __int16 pitch; /* 0x02 */
} direction_playback_controller;
