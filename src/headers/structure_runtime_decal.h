#pragma once
/* structure_runtime_decal — one baked decal record in the structure BSP's runtime-decals block
 * (16 bytes, DB types_members-confirmed). */

#include <stdint.h>
#include "real_point3d.h"

typedef struct structure_runtime_decal
{
    real_point3d    position;      /* 0x00 */
    uint8_t palette_index; /* 0x0C — index into scenario decal palette */
    uint8_t unused;        /* 0x0D */
    char            yaw;           /* 0x0E */
    char            pitch;         /* 0x0F */
} structure_runtime_decal;         /* 16 bytes */
