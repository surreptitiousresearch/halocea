/* memory_block_lock @0x838030C8 */
#include "headers/memory_block.h"

void memory_block_lock(memory_block *block)
{
    block->bits |= 0x80000000u;
}
