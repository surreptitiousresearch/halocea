#pragma once
/* mass_point_datum — per-mass-point physics scratch record (304 bytes), DB-verified via types_members.
 * vehicle_update allocates arrays of these on the stack and hands them to the per-vehicle physics
 * updaters, physics_update_new (ground/water/air contact + collision response), and the effect routines. */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_plane3d.h"
#include "location.h"
#include "friction_datum.h"

typedef struct mass_point_datum
{
    unsigned int   flags;                       /* 0x00 */
    real_point3d   position;                    /* 0x04 */
    real_vector3d  forward;                     /* 0x10 */
    real_vector3d  left;                        /* 0x1C */
    real_vector3d  up;                          /* 0x28 */
    location       location;                    /* 0x34 */
    real_vector3d  radius;                      /* 0x3C */
    real_vector3d  velocity;                    /* 0x48 */
    real_vector3d  velocity_relative_to_ground; /* 0x54 */
    real_plane3d   ground_plane;                /* 0x60 */
    __int16        ground_material_type;        /* 0x70 */
    unsigned char _pad0[2]; /* db-verified padding */
    float          ground_depth;                /* 0x74 */
    __int16        water_material_type;         /* 0x78 */
    unsigned char _pad1[2]; /* db-verified padding */
    float          water_depth;                 /* 0x7C */
    float          normal_force_magnitude;      /* 0x80 */
    real_vector3d  normal_force;                /* 0x84 */
    friction_datum ground_friction;             /* 0x90 */
    float          water_pressure_magnitude;    /* 0xB4 */
    real_vector3d  water_pressure;              /* 0xB8 */
    friction_datum water_friction;              /* 0xC4 */
    friction_datum air_friction;                /* 0xE8 */
    real_vector3d  powered_force;               /* 0x10C */
    real_vector3d  force;                       /* 0x118 */
    real_vector3d  torque;                      /* 0x124 */
} mass_point_datum;   /* 304 bytes */
