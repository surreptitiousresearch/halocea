/* memory_pool_block_reallocate @0x8371BF38 — resize an existing pool block to `new_size` payload
 * bytes. If the rounded block still fits before the next block (or the pool end when it is the last
 * block), it is grown/shrunk in place; otherwise a new block is allocated, the payload copied over,
 * and the old block freed. The owner's payload pointer (*reference) is updated on relocation. Returns
 * 1 on success, 0 if a needed relocation could not be allocated. */

#include <stdint.h>
#include "headers/memory_pool.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);

extern uint8_t memory_pool_block_allocate(memory_pool *pool, void **reference, int size);
extern void memory_pool_block_free(memory_pool *pool, void **reference);

uint8_t memory_pool_block_reallocate(memory_pool *pool, void **reference, int new_size)
{
    int block_size = new_size + 24;
    memory_pool_block *block = (memory_pool_block *)((char *)*reference - 24);
    char *limit;

    if ( (block_size & 3) != 0 )
        block_size = (block_size | 3) + 1;   /* round up to 4 */

    limit = (char *)block->next_block;
    if ( !limit )
        limit = (char *)pool->base_address + pool->size;

    if ( (char *)block + block_size > limit )
    {
        void *new_payload;
        if ( !memory_pool_block_allocate(pool, &new_payload, new_size) )
            return 0;
        memcpy(new_payload, *reference, block->size - sizeof(memory_pool_block));
        memory_pool_block_free(pool, reference);
        ((memory_pool_block *)new_payload - 1)->reference = reference;
        *reference = new_payload;
        return 1;
    }

    /* fits in place: adjust free count and record the new block size */
    pool->free_size += block->size - block_size;
    block->size = block_size;
    return 1;
}
