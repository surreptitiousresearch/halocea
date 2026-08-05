#pragma once
/* contrail_point_datum — one live contrail point (56 bytes; DB-verified, matches headers_ref). */

#include <stdint.h>
#include "location.h"
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct contrail_point_datum
{
    int16_t         identifier;                 /* 0x00 */
    uint8_t flags;                      /* 0x02 */
    char            state_index;                /* 0x03 */
    float           time;                       /* 0x04 */
    float           delta;                      /* 0x08 */
    float           density;                    /* 0x0C */
    float           width;                      /* 0x10 */
    location        location;                   /* 0x14 */
    real_point3d    position;                   /* 0x1C */
    real_vector3d   velocity;                   /* 0x28 */
    int             next_contrail_point_index;  /* 0x34 */
} contrail_point_datum;                         /* 56 bytes */
