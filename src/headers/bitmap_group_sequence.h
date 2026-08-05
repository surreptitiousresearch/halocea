#pragma once
/* bitmap_group_sequence — one named animation/sprite sequence within a bitmap group (60 bytes). Either a run of
 * bitmaps (first_bitmap_index + bitmap_count) or a sprite sheet (the sprites block). Layout from the database. */

#include <stdint.h>
#include "tag_block.h"

typedef struct bitmap_group_sequence
{
    char      name[32];           /* 0x00 */
    int16_t   first_bitmap_index; /* 0x20 */
    int16_t   bitmap_count;       /* 0x22 */
    int       unused[4];          /* 0x24 */
    tag_block sprites;            /* 0x34 */
} bitmap_group_sequence;          /* 0x40 */
