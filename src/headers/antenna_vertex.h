#pragma once
/* antenna_vertex — one antenna_definition vertex record (128 bytes). Layout verbatim from the database type.
 * Stored in antenna_definition.vertices (128-byte stride). */

#include <stdint.h>
#include "real_euler_angles2d.h"
#include "real_argb_color.h"
#include "real_vector3d.h"

typedef struct antenna_vertex
{
    float                spring_coefficient;   /* 0x00 */
    int                  physics_unused[6];    /* 0x04 */
    real_euler_angles2d  angle_to_next;        /* 0x1C */
    float                length_to_next;       /* 0x24 */
    int16_t              sequence_index;       /* 0x28 */
    uint16_t     pad;                  /* 0x2A */
    real_argb_color      color;                /* 0x2C */
    real_argb_color      lod_color;            /* 0x3C */
    int                  unused[10];           /* 0x4C */
    real_vector3d        vector_to_next;       /* 0x74 */
} antenna_vertex;
