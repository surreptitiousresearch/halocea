#pragma once
/* bitmap_group — a 'bitm' tag: import settings plus the sequence and bitmap blocks. Layout from the database. */

#include "tag_block.h"
#include "tag_data.h"

typedef struct bitmap_group
{
    __int16  type;                  /* 0x00 */
    __int16  format;                /* 0x02 */
    __int16  usage;                 /* 0x04 */
    unsigned __int16 flags;         /* 0x06 */
    float    detail_fade;           /* 0x08 */
    float    sharpen_amount;        /* 0x0C */
    float    bump_height;           /* 0x10 */
    __int16  sprite_budget_size;    /* 0x14 */
    __int16  sprite_budget_count;   /* 0x16 */
    __int16  import_width;          /* 0x18 */
    __int16  import_height;         /* 0x1A */
    tag_data import_bitmap;         /* 0x1C */
    tag_data pixel_data;            /* 0x30 */
    float    smoothing_filter_size; /* 0x44 */
    float    alpha_bias;            /* 0x48 */
    __int16  mipmap_count;          /* 0x4C */
    __int16  sprite_usage;          /* 0x4E */
    __int16  sprite_spacing;        /* 0x50 */
    unsigned __int16 pad;           /* 0x52 */
    tag_block sequences;            /* 0x54 */
    tag_block bitmaps;              /* 0x60 */
} bitmap_group;                     /* 0x6C */
