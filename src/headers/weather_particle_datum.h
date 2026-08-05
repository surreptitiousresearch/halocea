#pragma once
/* weather_particle_datum — one live weather particle (84 bytes; DB-verified). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_argb_color.h"

typedef struct weather_particle_datum
{
    int16_t       identifier;           /* 0x00 */
    unsigned char _pad0[2]; /* db-verified padding */
    real_point3d  position;             /* 0x04 */
    real_vector3d velocity;             /* 0x10 */
    real_vector3d acceleration;         /* 0x1C */
    int16_t       sequence_index;       /* 0x28 */
    unsigned char _pad1[2]; /* db-verified padding */
    float         sprite_index;         /* 0x2C */
    float         rotation;             /* 0x30 */
    real_argb_color color;              /* 0x34 */
    float         radius;               /* 0x44 */
    float         rotation_rate;        /* 0x48 */
    float         animation_rate;       /* 0x4C */
    int           next_particle_index;  /* 0x50 */
} weather_particle_datum;               /* 84 bytes */
