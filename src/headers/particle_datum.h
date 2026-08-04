#pragma once
/* particle_datum — one live particle (112 bytes). Layout from the database. */

#include "location.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_argb_color.h"

typedef struct particle_datum
{
    __int16          identifier;                /* 0x00 */
    unsigned __int16 flags;                     /* 0x02 */
    int              definition_index;          /* 0x04 */
    int              object_index;              /* 0x08 */
    __int16          node_index;                /* 0x0C */
    unsigned __int8  state;                     /* 0x0E */
    unsigned __int8  local_player_index;        /* 0x0F */
    int              last_rendered_frame_index; /* 0x10 */
    float            life_time;                 /* 0x14 */
    float            life_span;                 /* 0x18 */
    float            frame_time;                /* 0x1C */
    float            frame_span;                /* 0x20 */
    __int16          sequence_index;            /* 0x24 */
    __int16          frame_index;               /* 0x26 */
    location         location;                  /* 0x28 */
    real_point3d     position;                  /* 0x30 */
    real_vector3d    direction;                 /* 0x3C */
    real_vector3d    translational_velocity;    /* 0x48 */
    float            rotation;                  /* 0x54 */
    float            angular_velocity;          /* 0x58 */
    float            radius;                    /* 0x5C */
    real_argb_color  color;                     /* 0x60 */
} particle_datum;                               /* 112 bytes */
