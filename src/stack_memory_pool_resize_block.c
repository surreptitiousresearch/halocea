/* stack_memory_pool_resize_block @0x838039C8 — resizes a stack-pool allocation: allocates fresh only
 * when growing past the block's current usable size (`bits & 0x7FFFFFFF`, minus the 16-byte header),
 * copying the old payload over and disposing the old block; shrinking (or an unallocated `reference`)
 * is handled without a new allocation. Clean decompile, matches applied prototype directly. */

#include <string.h>

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);
extern stack_memory_pool *stack_memory_pool_dispose_block(stack_memory_pool *pool, const memory_block *reference);

memory_block *stack_memory_pool_resize_block(stack_memory_pool *pool, memory_block *reference,
        unsigned int new_size)
{
    if (!new_size)
        return 0;

    if (!reference)
        return stack_memory_pool_allocate(pool, new_size);

    if ((reference->bits & 0x7FFFFFFF) - 16 >= new_size)
        return reference;

    memory_block *resized = stack_memory_pool_allocate(pool, new_size);
    if (resized)
    {
        memcpy(resized + 1, reference + 1, (reference->bits & 0x7FFFFFFF) - 16);
        stack_memory_pool_dispose_block(pool, reference);
    }
    return resized;
}
