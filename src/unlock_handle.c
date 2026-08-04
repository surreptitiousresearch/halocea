/* unlock_handle @0x83803448 — matches the original disasm exactly, including a real null-pointer
 * dereference in the original binary: if the pool is empty (number_of_blocks==0) or the handle isn't found
 * after scanning every block, the "found" pointer is still null when its `bits` field is cleared
 * (disasm_range(0x83803448,0x838034AC) confirms both paths fall into the same `lwz r11,0(r7)` with r7 never
 * reassigned). Reproduced faithfully — this path is presumably unreachable in practice. */

#include "headers/stack_memory_pool.h"

void unlock_handle(stack_memory_pool *pool, memory_block *h)
{
    unsigned int number_of_blocks = pool->number_of_blocks;
    memory_block *found = 0;

    if ( number_of_blocks )
    {
        memory_block **block = pool->blocks;
        unsigned int index = 0;

        while ( !*block || &(*block)[1] != h )
        {
            if ( ++index >= number_of_blocks )
            {
                found->bits &= ~0x80000000;
                return;
            }
            block++;
        }
        found = pool->blocks[index];
    }
    found->bits &= ~0x80000000;
}
