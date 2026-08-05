#pragma once
/* glow_particle — one particle in a glow_datum's trailing-particle list. Layout from the database. */

#include <stdint.h>
#include "real_argb_color.h"
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct glow_particle
{
    int16_t          identifier;             /* 0x00 */
    int16_t          parent_marker_index;    /* 0x02 */
    int              index;                  /* 0x04 */
    float            initial_angle;          /* 0x08 */
    real_argb_color  color;                  /* 0x0C */
    float            distance_to_object;     /* 0x1C */
    float            initial_size;           /* 0x20 */
    float            present_size;           /* 0x24 */
    float            t;                      /* 0x28 */
    real_point3d     position;               /* 0x2C */
    real_vector3d    initial_velocity;       /* 0x38 */
    real_vector3d    present_velocity;       /* 0x44 */
    int16_t          ticks_in_existence;     /* 0x50 */
    int16_t          lifetime;                /* 0x52 */
    unsigned int     flags;                  /* 0x54 */
    float            fade;                   /* 0x58 */
    struct glow_particle *next;              /* 0x5C */
    struct glow_particle *previous;          /* 0x60 */
} glow_particle; /* 100 bytes */
