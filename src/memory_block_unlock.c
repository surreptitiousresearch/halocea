#include "headers/memory_block.h"

void memory_block_unlock(memory_block *block)
{
    block->bits &= ~0x80000000u;
}
