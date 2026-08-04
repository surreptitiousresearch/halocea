#pragma once
/* actor_move_orders — member of actor_orders at relative 0x18, 84 bytes (per the authoritative
 * actor_datum structure dump). Per structure dump from C:/halo/headers_ref/. */

#include "actor_destination_orders.h"
#include "actor_animation_orders.h"
#include "real_vector3d.h"
#include "real_vector2d.h"

typedef struct actor_move_orders
{
    actor_destination_orders destination; /* 0x00 (24 B) */
    actor_animation_orders animation;     /* 0x18 (12 B) */
    unsigned char dive_into_cover;        /* 0x24 */
    unsigned char emerge_from_cover;      /* 0x25 */
    unsigned char stationary_crouch;      /* 0x26 */
    unsigned char moving_crouch;          /* 0x27 */
    unsigned char panicked;               /* 0x28 */
    unsigned char flaming;                /* 0x29 */
    unsigned char move_face_exactly;      /* 0x2A */
    unsigned char _pad2B;                 /* 0x2B */
    __int16 override_movement_type;       /* 0x2C */
    __int16 override_movement_facing;     /* 0x2E */
    unsigned char override_movement_direction; /* 0x30 */
    unsigned char _pad31[3];              /* 0x31 */
    real_vector3d override_movement_direction_vector; /* 0x34 (12 B) */
    unsigned char jump;                   /* 0x40 */
    unsigned char jump_leap;              /* 0x41 */
    unsigned char jump_targeted;          /* 0x42 */
    unsigned char _pad43;                 /* 0x43 */
    real_vector2d jump_alignment_vector;  /* 0x44 (8 B) */
    float jump_target_horizontal_vel;     /* 0x4C */
    float jump_target_vertical_vel;       /* 0x50 */
} actor_move_orders; /* 84 bytes */
