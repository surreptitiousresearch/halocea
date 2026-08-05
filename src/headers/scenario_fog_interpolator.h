/* scenario_fog_interpolator — per-local-player fog interpolation state, eased toward the active sky's
 * atmospheric fog each frame. Layout from the database (scenario_fog_interpolator). */
#pragma once

#include <stdint.h>
#include "real_point3d.h"
#include "real_rgb_color.h"

typedef struct scenario_fog_interpolator
{
    uint8_t valid;                            /* 0x00 */
    char            _pad01[3];                         /* 0x01 */
    real_point3d    point;                             /* 0x04 — last camera point the state was eased toward */
    float           atmospheric_fog_z_near;            /* 0x10 */
    float           atmospheric_fog_z_far;             /* 0x14 */
    float           atmospheric_fog_maximum_density;   /* 0x18 */
    real_rgb_color  atmospheric_fog_color;             /* 0x1C */
    float           screen_external_intensity;         /* 0x28 */
} scenario_fog_interpolator;                                  /* 44 bytes */
