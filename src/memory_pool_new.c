/* memory_pool_new @ 0x8371BDD8 — heap-allocate and init a memory_pool */

#include <string.h>
#include "headers/memory_pool.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
/* strncpy declared by <string.h> */

memory_pool *memory_pool_new(const char *name, int size)
{
    memory_pool *pool = dlMalloc(
        size + 56, "D:\\Projects\\code\\HCEX\\sources\\memory\\memory_pool.c", 0x46);
    if ( pool )
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
    return pool;
}
