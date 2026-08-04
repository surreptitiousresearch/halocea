#pragma once
/* animation_playback_controller — the 3 yaw/pitch direction controllers (facing/aiming/looking) driven
 * during recorded-animation event-stream playback. DB types_members-confirmed layout (12 bytes). */

#include "direction_playback_controller.h"

typedef struct animation_playback_controller
{
    direction_playback_controller facing_control;  /* 0x00 */
    direction_playback_controller aiming_control;  /* 0x04 */
    direction_playback_controller looking_control; /* 0x08 */
} animation_playback_controller; /* 12 bytes */
