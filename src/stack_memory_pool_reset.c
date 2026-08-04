/* stack_memory_pool_reset @0x83803068 — clear a stack memory pool back to empty: zero the block-pointer table
 * and the pool header, then restore the persistent fields (name, base address, block count, pool size) and
 * point the first block slot back at the block table base. */

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"

extern void *memset(void *dst, int value, unsigned int size);

void stack_memory_pool_reset(stack_memory_pool *pool)
{
    int             number_of_blocks = pool->number_of_blocks;
    memory_block  **blocks           = pool->blocks;
    char           *name             = pool->name;
    unsigned char  *base_address     = pool->base_address;
    unsigned int    pool_size        = pool->pool_size;

    memset(pool->blocks, 0, 4 * number_of_blocks);
    memset(pool, 0, sizeof(stack_memory_pool));

    pool->name             = name;
    pool->base_address     = base_address;
    pool->number_of_blocks = number_of_blocks;
    pool->pool_size        = pool_size;
    pool->blocks[0]        = (memory_block *)blocks;
}
