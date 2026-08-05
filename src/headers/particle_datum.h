#pragma once
/* particle_datum — one live particle (112 bytes). Layout from the database. */

#include <stdint.h>
#include "location.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_argb_color.h"

typedef struct particle_datum
{
    int16_t          identifier;                /* 0x00 */
    uint16_t flags;                     /* 0x02 */
    int              definition_index;          /* 0x04 */
    int              object_index;              /* 0x08 */
    int16_t          node_index;                /* 0x0C */
    uint8_t  state;                     /* 0x0E */
    uint8_t  local_player_index;        /* 0x0F */
    int              last_rendered_frame_index; /* 0x10 */
    float            life_time;                 /* 0x14 */
    float            life_span;                 /* 0x18 */
    float            frame_time;                /* 0x1C */
    float            frame_span;                /* 0x20 */
    int16_t          sequence_index;            /* 0x24 */
    int16_t          frame_index;               /* 0x26 */
    location         location;                  /* 0x28 */
    real_point3d     position;                  /* 0x30 */
    real_vector3d    direction;                 /* 0x3C */
    real_vector3d    translational_velocity;    /* 0x48 */
    float            rotation;                  /* 0x54 */
    float            angular_velocity;          /* 0x58 */
    float            radius;                    /* 0x5C */
    real_argb_color  color;                     /* 0x60 */
} particle_datum;                               /* 112 bytes */
