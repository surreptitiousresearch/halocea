#pragma once
/* new_particle_data — the fully-resolved spawn parameters for a single particle (particles.c). 92 bytes;
 * layout from the database. */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_argb_color.h"

typedef struct new_particle_data
{
    int             definition_index;          /* 0x00 */
    int             object_index;              /* 0x04 — attaching object, -1 if free */
    __int16         node_index;                /* 0x08 */
    __int16         local_player_index;        /* 0x0A */
    unsigned __int8 attached_to_local_player;  /* 0x0C */
    unsigned __int8 dont_draw_first_person;    /* 0x0D */
    unsigned __int8 dont_draw_third_person;    /* 0x0E */
    char            _pad0F;                    /* 0x0F */
    real_point3d    position;                  /* 0x10 */
    real_vector3d   direction;                 /* 0x1C */
    real_vector3d   velocity;                  /* 0x28 */
    real_vector3d   initial_impulse;           /* 0x34 */
    float           rotation;                  /* 0x40 */
    float           angular_velocity;          /* 0x44 */
    float           radius;                    /* 0x48 */
    real_argb_color color;                     /* 0x4C */
} new_particle_data;                           /* 92 bytes */
