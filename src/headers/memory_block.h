#pragma once
/* memory_block — allocation header stored immediately before a stack-pool user pointer.
 * The low 31 bits of `bits` are the block size; the high bit (0x80000000) marks in-use. */

typedef struct memory_block
{
    unsigned int         bits;            /* 0x0 — size (& 0x7FFFFFFF) | in-use (0x80000000) */
    unsigned int         index;           /* 0x4 — slot in the pool's block table */
    struct memory_block *previous_block;  /* 0x8 */
    struct memory_block *next_block;      /* 0xC */
} memory_block;                           /* 16 bytes */

/* `bits` accessors: the high bit marks the block in-use, the low 31 bits are the block size. */
#define MEMORY_BLOCK_IN_USE_FLAG       0x80000000
#define MEMORY_BLOCK_IS_IN_USE(bits)   (((bits) & MEMORY_BLOCK_IN_USE_FLAG) != 0)
#define MEMORY_BLOCK_SIZE(bits)        ((bits) & 0x7FFFFFFF)
