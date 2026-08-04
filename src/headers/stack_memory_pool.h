#pragma once
/* stack_memory_pool — a doubly-linked-block stack allocator (stack_memory_pool.c). */

#include "memory_block.h"

/* DB anonymous struct _47B76D5965B7D95DF62E16F350AD30D6 — the pool statistics block
 * ($-name kept verbatim; $ in identifiers is an MSVC extension). */
typedef struct _47B76D5965B7D95DF62E16F350AD30D6
{
    int          bytes_allocated;         /* 0x00 */
    int          peak_bytes_allocated;    /* 0x04 */
    unsigned int blocks_used;             /* 0x08 */
    unsigned int peak_blocks_used;        /* 0x0C */
    int          largest_block_allocated; /* 0x10 */
} _47B76D5965B7D95DF62E16F350AD30D6;                                        /* 20 bytes */
typedef struct _47B76D5965B7D95DF62E16F350AD30D6 stack_memory_pool_statistics;

typedef struct stack_memory_pool
{
    char                         *name;             /* 0x00 */
    unsigned char                *base_address;     /* 0x04 */
    unsigned int                  pool_size;        /* 0x08 */
    int                           number_of_blocks; /* 0x0C */
    int                           next_block_index; /* 0x10 */
    struct _47B76D5965B7D95DF62E16F350AD30D6 statistics; /* 0x14 */
    unsigned char                 locked;           /* 0x28 */
    unsigned char                 _pad29[3];        /* 0x29 — db-verified padding */
    memory_block                 *first_block;      /* 0x2C */
    memory_block                 *last_block;       /* 0x30 */
    memory_block                 *blocks[];         /* 0x34 — flexible block table */
} stack_memory_pool;

/* stack_memory_pool_medium — a stack_memory_pool with a fixed 4096-entry block table
 * (DB type stack_memory_pool_medium, 16436 bytes). Layout identical to stack_memory_pool
 * through 0x34, then blocks[4096]. */
typedef struct stack_memory_pool_medium
{
    char                         *name;             /* 0x00 */
    unsigned char                *base_address;     /* 0x04 */
    unsigned int                  pool_size;        /* 0x08 */
    int                           number_of_blocks; /* 0x0C */
    int                           next_block_index; /* 0x10 */
    struct _47B76D5965B7D95DF62E16F350AD30D6 statistics; /* 0x14 */
    unsigned char                 locked;           /* 0x28 */
    unsigned char                 _pad29[3];        /* 0x29 — db-verified padding */
    memory_block                 *first_block;      /* 0x2C */
    memory_block                 *last_block;       /* 0x30 */
    memory_block                 *blocks[4096];     /* 0x34 — fixed block table */
} stack_memory_pool_medium;                         /* 16436 bytes */
