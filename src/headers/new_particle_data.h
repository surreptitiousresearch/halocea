#pragma once
/* new_particle_data — the fully-resolved spawn parameters for a single particle (particles.c). 92 bytes;
 * layout from the database. */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_argb_color.h"

typedef struct new_particle_data
{
    int             definition_index;          /* 0x00 */
    int             object_index;              /* 0x04 — attaching object, -1 if free */
    int16_t         node_index;                /* 0x08 */
    int16_t         local_player_index;        /* 0x0A */
    uint8_t attached_to_local_player;  /* 0x0C */
    uint8_t dont_draw_first_person;    /* 0x0D */
    uint8_t dont_draw_third_person;    /* 0x0E */
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
