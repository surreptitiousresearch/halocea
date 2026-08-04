#pragma once
/* aim_assist_target — result of an aim-assist query: the chosen target object and the computed assist
 * geometry. DB layout (56 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct aim_assist_target
{
    int           object_index;    /* 0x00 */
    real_point3d  position;        /* 0x04 */
    real_vector3d vector;          /* 0x10 */
    real_vector3d direction;       /* 0x1C */
    float         distance;        /* 0x28 */
    float         angle;           /* 0x2C */
    float         autoaim_level;   /* 0x30 */
    float         magnetism_level; /* 0x34 */
} aim_assist_target;               /* 56 bytes */
