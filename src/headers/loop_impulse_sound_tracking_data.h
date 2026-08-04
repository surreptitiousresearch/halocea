/* loop_impulse_sound_tracking_data — the per-spawn tracking payload carried by a looping sound's
 * detail impulse so the channel can be re-located each frame: the random position offset (12 bytes). */
#pragma once

#include "real_vector3d.h"

typedef struct loop_impulse_sound_tracking_data
{
    real_vector3d position_offset; /* 0x00 */
} loop_impulse_sound_tracking_data;
