#include "headers/memory_pool.h"
#include <string.h>

extern void dlFree(void *ptr);

void memory_pool_delete(memory_pool *pool)
{
    memset(pool, 0, sizeof(memory_pool));
    dlFree(pool);
}
