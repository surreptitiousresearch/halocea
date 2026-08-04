#pragma once
/* bitmap_group_sprite — one sprite within a sprite-sheet sequence (32 bytes): which bitmap it lives in, its
 * normalized bounds, and a registration point. Layout from the database. */

#include "real_rectangle2d.h"
#include "real_point2d.h"

typedef struct bitmap_group_sprite
{
    __int16          bitmap_index;       /* 0x00 */
    __int16          bitmap_pad;         /* 0x02 */
    int              unused;             /* 0x04 */
    real_rectangle2d bounds;             /* 0x08 */
    real_point2d     registration_point; /* 0x18 */
} bitmap_group_sprite;                   /* 0x20 */
