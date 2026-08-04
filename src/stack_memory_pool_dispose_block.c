/* stack_memory_pool_dispose_block @ 0x83803138 — unlink a block from the pool's doubly linked
 * list and clear its slot in the block table, fixing up the first/last pointers and the
 * next-free-slot hint. */

#include "headers/stack_memory_pool.h"

/* Returns `pool`: r3 (the pool argument) is never clobbered, and callers (dispose_handle,
 * dispose_pointer) consume it as a pointer base immediately after the call with no reload. */
stack_memory_pool *stack_memory_pool_dispose_block(stack_memory_pool *pool, const memory_block *reference)
{
    memory_block *previous_block = reference->previous_block;
    unsigned int index = reference->index;

    if ( previous_block )
        previous_block->next_block = reference->next_block;
    if ( reference->next_block )
        reference->next_block->previous_block = reference->previous_block;

    if ( reference == pool->first_block )
        pool->first_block = reference->next_block;
    if ( reference == pool->last_block )
        pool->last_block = reference->previous_block;

    pool->blocks[index] = 0;
    pool->next_block_index = (pool->first_block == 0) ? 0 : index;
    return pool;
}
