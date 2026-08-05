#pragma once
/* bitmap_group — a 'bitm' tag: import settings plus the sequence and bitmap blocks. Layout from the database. */

#include <stdint.h>
#include "tag_block.h"
#include "tag_data.h"

typedef struct bitmap_group
{
    int16_t  type;                  /* 0x00 */
    int16_t  format;                /* 0x02 */
    int16_t  usage;                 /* 0x04 */
    uint16_t flags;         /* 0x06 */
    float    detail_fade;           /* 0x08 */
    float    sharpen_amount;        /* 0x0C */
    float    bump_height;           /* 0x10 */
    int16_t  sprite_budget_size;    /* 0x14 */
    int16_t  sprite_budget_count;   /* 0x16 */
    int16_t  import_width;          /* 0x18 */
    int16_t  import_height;         /* 0x1A */
    tag_data import_bitmap;         /* 0x1C */
    tag_data pixel_data;            /* 0x30 */
    float    smoothing_filter_size; /* 0x44 */
    float    alpha_bias;            /* 0x48 */
    int16_t  mipmap_count;          /* 0x4C */
    int16_t  sprite_usage;          /* 0x4E */
    int16_t  sprite_spacing;        /* 0x50 */
    uint16_t pad;           /* 0x52 */
    tag_block sequences;            /* 0x54 */
    tag_block bitmaps;              /* 0x60 */
} bitmap_group;                     /* 0x6C */
