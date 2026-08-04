#pragma once
/* structure_runtime_decal — one baked decal record in the structure BSP's runtime-decals block
 * (16 bytes, DB types_members-confirmed). */

#include "real_point3d.h"

typedef struct structure_runtime_decal
{
    real_point3d    position;      /* 0x00 */
    unsigned __int8 palette_index; /* 0x0C — index into scenario decal palette */
    unsigned __int8 unused;        /* 0x0D */
    char            yaw;           /* 0x0E */
    char            pitch;         /* 0x0F */
} structure_runtime_decal;         /* 16 bytes */
