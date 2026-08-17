/* stack_memory_pool_lock_block @0x838031B8 */
#include "headers/stack_memory_pool.h"

int * stack_memory_pool_lock_block(stack_memory_pool *pool)
{
    pool->name = (char *)((int)pool->name | 0x80000000);
    return &pool->next_block_index;
}
