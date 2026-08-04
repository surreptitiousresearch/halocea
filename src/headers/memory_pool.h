#pragma once
/* memory_pool — generic block allocator header (memory_pool.c); 56-byte header
 * followed by the pool payload. */

#include "memory_pool_block.h"

typedef struct memory_pool
{
    unsigned int       signature;     /* 0x00 — 'pool' (1886351212) */
    char               name[32];      /* 0x04 */
    void              *base_address;  /* 0x24 — &pool[1] */
    int                size;          /* 0x28 */
    int                free_size;     /* 0x2C */
    memory_pool_block *first_block;   /* 0x30 */
    memory_pool_block *last_block;    /* 0x34 */
} memory_pool;                        /* 56 bytes */
