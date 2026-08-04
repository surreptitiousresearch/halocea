#pragma once
/* structure_mirror — one reflective surface in a structure cluster (64 bytes; DB-verified). */

#include "real_plane3d.h"
#include "tag_reference.h"
#include "tag_block.h"

typedef struct structure_mirror
{
    real_plane3d  plane;        /* 0x00 */
    int           unused[5];    /* 0x10 */
    tag_reference shader;       /* 0x24 */
    tag_block     points;       /* 0x34 */
} structure_mirror;             /* 64 bytes */
