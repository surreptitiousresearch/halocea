/* memory_pool_initialize @ 0x8371BBB0 — initialize a memory_pool header in place */

#include <string.h>

#include "headers/memory_pool.h"

void memory_pool_initialize(memory_pool *pool, const char *name, int size)
{
    memset(pool, 0, sizeof(memory_pool));
    pool->signature = 0x706F6F6Cu;  /* 'pool' */
    strncpy(pool->name, name, 0x1F);
    pool->size = size;
    pool->free_size = size;
    pool->base_address = &pool[1];
    pool->first_block = 0;
    pool->last_block = 0;
}
