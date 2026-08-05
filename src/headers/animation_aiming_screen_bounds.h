#pragma once
#include <stdint.h>
/* animation_aiming_screen_bounds — a model_animation_graph unit_seat/weapon_class entry's yaw/pitch overlay
 * ramp: for each of the 4 directions, how many frames the overlay animation spans and the per-frame angular
 * delta. aiming_screen_apply consumes this alongside an animation to blend a yaw/pitch direction into node
 * orientations. Layout DB-verified (24 bytes). */

typedef struct animation_aiming_screen_bounds
{
    float   negative_yaw_delta;           /* 0x00 */
    float   positive_yaw_delta;           /* 0x04 */
    int16_t negative_yaw_frame_count;     /* 0x08 */
    int16_t positive_yaw_frame_count;     /* 0x0A */
    float   negative_pitch_delta;         /* 0x0C */
    float   positive_pitch_delta;         /* 0x10 */
    int16_t negative_pitch_frame_count;   /* 0x14 */
    int16_t positive_pitch_frame_count;   /* 0x16 */
} animation_aiming_screen_bounds;         /* 0x18 = 24 bytes */
