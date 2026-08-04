#pragma once
/* actor_input_data — the actor's per-tick sampled view of its unit (vehicle state, vitality, pose),
 * embedded in actor_datum at +0x120 (168 bytes). Full DB layout. */

#include "actor_position_data.h"
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct actor_input_data
{
    actor_position_data position;                       /* 0x00 */
    int                 vehicle_index;                  /* 0x38 */
    unsigned __int8     in_midair;                      /* 0x3C */
    unsigned __int8     underwater;                     /* 0x3D */
    __int16             vehicle_driver_type;            /* 0x3E */
    unsigned __int8     vehicle_passenger;              /* 0x40 */
    unsigned __int8     vehicle_gunner;                 /* 0x41 */
    unsigned __int8     vehicle_gunner_bombardment;     /* 0x42 */
    unsigned char       _pad43;                         /* 0x43 */
    int                 pathfinding_surface_index;      /* 0x44 */
    real_point3d        pathfinding_point;              /* 0x48 */
    real_vector3d       facing_vector;                  /* 0x54 */
    real_vector3d       aiming_vector;                  /* 0x60 */
    real_vector3d       looking_vector;                 /* 0x6C */
    real_vector3d       looking_left_vector;            /* 0x78 */
    real_vector3d       looking_up_vector;              /* 0x84 */
    int                 delayed_attached_projectile_index; /* 0x90 */
    unsigned __int8     melee_attacker_attached;        /* 0x94 */
    unsigned __int8     burning_to_death;               /* 0x95 */
    unsigned char       _pad96[2];                      /* 0x96 */
    float               body_vitality;                  /* 0x98 */
    float               shield_vitality;                /* 0x9C */
    float               recent_body_damage;             /* 0xA0 */
    float               recent_shield_damage;           /* 0xA4 */
} actor_input_data;                                     /* 168 bytes */
