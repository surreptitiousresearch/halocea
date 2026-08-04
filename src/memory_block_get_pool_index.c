/* memory_block_get_pool_index @0x83803130 — accessor for memory_block's index (slot in the pool's block table). */

#include "headers/memory_block.h"

unsigned int memory_block_get_pool_index(memory_block *block)
{
    return block->index;
}
