/* wind_state — per-weather-palette-entry runtime wind, derived from the weather definition plus a
 * random-walk variance each tick (wind.c). */
#pragma once

#include <stdint.h>
#include "real_euler_angles2d.h"
#include "real_vector3d.h"

typedef struct wind_state
{
    uint8_t     valid;             /* 0x00 — 0 when the palette entry has no weather tag */
    unsigned char _pad0[3]; /* db-verified padding */
    float               velocity_variance; /* 0x04 — random-walk scalar, clamped [0,1] */
    real_euler_angles2d angular_variance;  /* 0x08 — random-walk yaw/pitch, clamped [-1,1] */
    float               velocity;          /* 0x10 — interpolated scalar wind speed */
    real_vector3d       velocity3d;        /* 0x14 — final world-space wind vector */
} wind_state;                              /* 32 bytes */
