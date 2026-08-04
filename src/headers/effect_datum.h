#pragma once
/* effect_datum — one live effect instance (effects.c). 252 bytes; layout from the database. */

#include "location.h"
#include "real_rgb_color.h"
#include "real_vector3d.h"
#include "effect_vector_field.h"

typedef struct effect_datum
{
    __int16             identifier;             /* 0x00 */
    unsigned __int16    flags;                  /* 0x02 */
    int                 definition_index;       /* 0x04 */
    __int16             scale_a_function_index; /* 0x08 */
    __int16             scale_b_function_index; /* 0x0A */
    __int16             change_color_index;     /* 0x0C */
    unsigned char _pad0[2]; /* db-verified padding */
    location            location;               /* 0x10 */
    real_rgb_color      color;                  /* 0x18 */
    real_vector3d       velocity;               /* 0x24 */
    effect_vector_field impulse_field;          /* 0x30 */
    int                 object_index;           /* 0x3C */
    int                 owner_object_index;     /* 0x40 */
    float               scale_a;                /* 0x44 */
    float               scale_b;                /* 0x48 */
    __int16             local_player_index;     /* 0x4C */
    __int16             event_index;            /* 0x4E */
    float               event_time;             /* 0x50 */
    float               event_duration;         /* 0x54 */
    float               last_event_fraction;    /* 0x58 */
    int                 location_datum_indices[32]; /* 0x5C */
    unsigned __int8     particles_counts[32];   /* 0xDC */
} effect_datum;                                 /* 252 bytes */
