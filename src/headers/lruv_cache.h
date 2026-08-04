#pragma once

#include "data_array.h"

/* LRU/validation cache header (68 bytes); block storage follows inline. */
typedef struct lruv_cache
{
    char name[32];                                  // 0x00
    void (*delete_block_proc)(int);                 // 0x20
    unsigned char (*locked_block_proc)(int);        // 0x24
    int page_count;                                 // 0x28
    int page_size_bits;                             // 0x2C
    unsigned int frame_index;                       // 0x30
    int first_block_index;                          // 0x34
    int last_block_index;                           // 0x38
    data_array *blocks;                             // 0x3C
    unsigned int signature;                         // 0x40
} lruv_cache;
