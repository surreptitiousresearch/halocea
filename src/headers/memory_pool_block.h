#pragma once
/* memory_pool_block — 24-byte block header prefixing each allocation in a memory_pool. The payload
 * follows immediately (block[1]). header/trailer signatures bracket the block for heap validation. */

typedef struct memory_pool_block
{
    unsigned int              header_signature;  /* 0x00 — 'head' (1751474532) */
    int                       size;              /* 0x04 — total block size incl. this header */
    void                    **reference;         /* 0x08 — back-pointer the owner stores the payload in */
    struct memory_pool_block *next_block;        /* 0x0C */
    struct memory_pool_block *previous_block;    /* 0x10 */
    unsigned int              trailer_signature; /* 0x14 — 'tail' (1952541036) */
} memory_pool_block;                             /* 24 bytes */
