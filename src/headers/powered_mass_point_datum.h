#pragma once
/* powered_mass_point_datum — per-powered-mass-point physics scratch record (96 bytes). The friction /
 * lift / thrust fractions plus the powered rotation (a quaternion + its derived matrix) are filled by
 * the per-vehicle physics updaters and consumed by physics_update. */

#include "real_quaternion.h"
#include "real_matrix4x3.h"

typedef struct powered_mass_point_datum
{
    float           ground_friction_velocity; /* 0x00 */
    float           water_friction_velocity;  /* 0x04 */
    float           air_friction_velocity;     /* 0x08 */
    float           water_lift_ratio;          /* 0x0C */
    float           air_lift_ratio;            /* 0x10 */
    float           thrust_fraction;           /* 0x14 */
    float           antigrav_fraction;         /* 0x18 */
    real_quaternion rotation;                 /* 0x1C */
    real_matrix4x3  rotation_matrix;          /* 0x2C */
} powered_mass_point_datum;                   /* 96 bytes */
