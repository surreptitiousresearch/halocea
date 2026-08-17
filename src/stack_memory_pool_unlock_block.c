/* stack_memory_pool_unlock_block @0x838031D0 */
#include "headers/stack_memory_pool.h"

void stack_memory_pool_unlock_block(stack_memory_pool *pool, memory_block *reference)
{
    /* clears lock bit from pool->name pointer; matches PPC: lwz/rlwinm/stw clearing bit 31 */
    pool->name = (char *)((int)pool->name & ~0x80000000);
}
