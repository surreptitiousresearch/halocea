#pragma once

#include <stdint.h>
#include "real_point3d.h"
#include "real_point2d.h"
#include "bitmap_group_sequence.h"
#include "bitmap_group_sprite.h"

typedef struct decal_editor_geometry
{
    real_point3d positions[4];                 /* 0x00 */
    real_point2d texcoords[4];                  /* 0x30 */
    const bitmap_group_sequence *sequence;       /* 0x50 */
    const bitmap_group_sprite *sprite;           /* 0x54 */
    int16_t bitmap_index;                        /* 0x58 */
    char pad_5A[2];                              /* 0x5A */
} decal_editor_geometry; /* 92 bytes */
