/* memory_block_get_real_size @0x83803108 — returns memory_block::bits with the in-use bit (0x80000000)
 * masked off. */

#include "headers/memory_block.h"

unsigned int memory_block_get_real_size(memory_block *block)
{
    return block->bits & 0x7FFFFFFF;
}
