#include "headers/stack_memory_pool.h"

void lock_handle(stack_memory_pool *pool, void ***h)
{
    void **next_handle = *h + 4;
    **h = (void *)((unsigned int)**h | 0x80000000);
    *h = next_handle;
}
