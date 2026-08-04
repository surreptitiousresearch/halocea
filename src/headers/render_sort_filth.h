#pragma once

typedef struct render_sort_filth
{
    short *prev_group_presorted_index_reference; /* 0x00 */
    short *next_group_presorted_index_reference; /* 0x04 */
    short  group_index;                          /* 0x08 */
    short  next_part_index;                      /* 0x0A */
    short  part_index;                           /* 0x0C */
    unsigned short pad;                          /* 0x0E */
} render_sort_filth;                             /* 0x10 (16) */
