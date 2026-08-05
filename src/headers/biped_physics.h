#pragma once
/* biped_physics — the scratch input/output block that drives one biped's per-frame movement/physics
 * integration. biped_update_moving[_simulated] fills the input half (current state, desired movement,
 * terrain-response constants) then calls biped_update_physics, which writes the output half (new
 * position/velocity, resolved support/elevator surfaces, landing velocity). Layout DB-verified
 * (types_members, 204 bytes). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_plane3d.h"

typedef struct biped_physics
{
    int              biped_index;                     /* 0x00 */
    uint16_t in_flags;                        /* 0x04 */
    char             _pad06[2];                        /* 0x06 */
    real_point3d     position;                        /* 0x08 */
    real_vector3d    forward;                         /* 0x14 */
    real_vector3d    aiming;                          /* 0x20 */
    real_vector3d    velocity;                        /* 0x2C */
    float            crouch_velocity;                 /* 0x38 */
    real_vector3d    movement_desired;                /* 0x3C */
    float            movement_penalty;                /* 0x48 */
    float            acceleration_maximum;            /* 0x4C */
    float            airborne_acceleration_maximum;   /* 0x50 */
    float            height;                          /* 0x54 */
    float            width;                           /* 0x58 */
    float            ground_tangential_velocity_max;  /* 0x5C */
    float            ground_tangential_angle;         /* 0x60 */
    float            minimum_normal_k;                /* 0x64 */
    float            downhill_k0;                     /* 0x68 */
    float            downhill_k1;                     /* 0x6C */
    float            downhill_velocity_scale;         /* 0x70 */
    float            uphill_k0;                       /* 0x74 */
    float            uphill_k1;                       /* 0x78 */
    float            uphill_velocity_scale;           /* 0x7C */
    real_plane3d     ground_plane;                    /* 0x80 */
    int              existing_support_surface_index;  /* 0x90 */
    float            gravity;                         /* 0x94 */
    int              bumped_object_index;             /* 0x98 */
    int              elevator_object_index;           /* 0x9C */
    uint16_t out_flags;                       /* 0xA0 */
    char             _padA2[2];                        /* 0xA2 */
    int              support_surface_index;           /* 0xA4 */
    int              stick_surface_index;             /* 0xA8 */
    real_point3d     new_position;                    /* 0xAC */
    real_vector3d    new_velocity;                    /* 0xB8 */
    float            landing_velocity;                /* 0xC4 */
    float            collision_velocity;              /* 0xC8 */
} biped_physics;                                       /* 204 bytes */
