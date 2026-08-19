/* memory_pool_get_free_size @0x8371BD00 — return the number of free bytes remaining in a memory pool. */

#include "headers/memory_pool.h"

int memory_pool_get_free_size(memory_pool *pool)
{
    return pool->free_size;
}
