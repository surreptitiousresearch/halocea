#include "headers/memory_pool.h"

memory_pool_block * memory_pool_block_new(memory_pool *pool, int size)
{
    char *base_address = pool->last_block
        ? (char *)pool->last_block + pool->last_block->size
        : (char *)pool->base_address;

    return (base_address + size <= (char *)pool->base_address + pool->size) ? (memory_pool_block *)base_address : 0;
}
