#pragma once
/* pathfinding_sphere — one element of collision_model.pathfinding_spheres (32 bytes; DB-verified).
   node_index (@0), center (@16) and radius (@28) all confirmed: the old note claiming
   obstacles_get_discs_in_sphere reads the point at +8 was a mistranslation — the decompiler's
   base pointer there is an unsigned __int16*, so its "+8" is byte offset 16 = center. */

#include "real_point3d.h"

typedef struct pathfinding_sphere
{
    __int16          node_index; /* 0x00 */
    unsigned __int16 pad;        /* 0x02 */
    int              unused[3];  /* 0x04 */
    real_point3d     center;     /* 0x10 */
    float            radius;     /* 0x1C */
} pathfinding_sphere;            /* 32 bytes */
