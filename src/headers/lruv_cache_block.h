#pragma once

/* lruv_cache_block — element of lruv_cache::blocks (28 bytes, DB-verified) */
typedef struct lruv_cache_block
{
    short          identifier;             /* 0x00 — data_array datum identifier */
    unsigned char  unused_flags[2];        /* 0x02 */
    int            page_count;             /* 0x04 */
    int            first_page_index;       /* 0x08 */
    int            next_block_index;       /* 0x0C */
    int            previous_block_index;   /* 0x10 */
    unsigned int   last_used_frame_index;  /* 0x14 */
    unsigned int   unused[1];              /* 0x18 */
} lruv_cache_block;                        /* 28 bytes */
