/* memory_pool_get_used_size @0x8371BD08 — bytes used so far: end of the last block minus the pool's base
 * address, or 0 if the pool is empty. */

#include "headers/memory_pool.h"

int memory_pool_get_used_size(memory_pool *pool)
{
    if (pool->last_block)
        return (int)pool->last_block + pool->last_block->size - (unsigned int)pool->base_address;
    return 0;
}
