/* stack_memory_pool_free_space_at_end_of_pool @0x838031E0 */
#include "headers/stack_memory_pool.h"

unsigned int stack_memory_pool_free_space_at_end_of_pool(stack_memory_pool *pool)
{
    if ( pool->first_block )
        return &pool->base_address[pool->pool_size - (pool->last_block->bits & 0x7FFFFFFF)]
             - (unsigned char *)pool->last_block;
    return pool->pool_size;
}
