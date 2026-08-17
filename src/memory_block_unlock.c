/* memory_block_unlock @0x838030D8 */
#include "headers/memory_block.h"

void memory_block_unlock(memory_block *block)
{
    block->bits &= ~0x80000000u;
}
