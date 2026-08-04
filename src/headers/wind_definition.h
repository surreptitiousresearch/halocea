#pragma once
/* wind_definition — the wind (`wind`) tag definition (DB-verified, 64 bytes). */

#include "real_euler_angles2d.h"

typedef struct wind_definition
{
    float velocity_lower_bound;         /* 0x00 */
    float velocity_upper_bound;         /* 0x04 */
    real_euler_angles2d variation_area; /* 0x08 */
    float local_variation_weight;       /* 0x10 */
    float local_variation_rate;         /* 0x14 */
    float damping;                      /* 0x18 */
    int unused[9];                      /* 0x1C */
} wind_definition;
