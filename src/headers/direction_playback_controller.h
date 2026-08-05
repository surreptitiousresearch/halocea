#pragma once
#include <stdint.h>
/* direction_playback_controller — accumulated yaw/pitch state driven by a sequence of
 * vector_char_difference_data ticks (e.g. cinematic/demo playback of recorded look input). */

typedef struct direction_playback_controller
{
    int16_t yaw;   /* 0x00 */
    int16_t pitch; /* 0x02 */
} direction_playback_controller;
