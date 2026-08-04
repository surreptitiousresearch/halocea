#pragma once
/* weapon_datum_network_data — networking baseline/update snapshot embedded in _weapon_datum.
 * Layout DB-verified against type 'weapon_datum_network_data' (44 bytes, 2026-07-14). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct weapon_datum_network_data
{
    real_point3d  position;                /* 0x00 */
    real_vector3d translational_velocity;  /* 0x0C */
    real_vector3d angular_velocity;        /* 0x18 */
    __int16       magazine_0_rounds_total; /* 0x24 */
    __int16       magazine_1_rounds_total; /* 0x26 */
    float         age;                     /* 0x28 */
} weapon_datum_network_data;               /* 44 bytes */
