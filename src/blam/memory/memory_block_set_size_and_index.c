/* memory_block_set_size_and_index @0x838030F8 — raw field setter for memory_block::bits/index. */

#include "headers/memory_block.h"

void memory_block_set_size_and_index(memory_block *block, unsigned int new_size, unsigned int index)
{
    block->bits = new_size;
    block->index = index;
}
