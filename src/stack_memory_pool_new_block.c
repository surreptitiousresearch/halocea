/* stack_memory_pool_new_block @0x83803968 — tail-call forwarder (`b stack_memory_pool_allocate`;
 * confirmed via disasm, not a bare blr). stack_memory_pool_allocate itself is not yet decompiled —
 * declared extern with its own DB prototype and called through with the same arguments. */

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);

memory_block * stack_memory_pool_new_block(stack_memory_pool *pool, unsigned int size)
{
    return stack_memory_pool_allocate(pool, size);
}
