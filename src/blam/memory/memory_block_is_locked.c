/* memory_block_is_locked @0x838030E8 — tests the in-use bit (0x80000000) of memory_block::bits. */

#include <stdint.h>
#include "headers/memory_block.h"

uint8_t memory_block_is_locked(memory_block *block)
{
    return block->bits >> 31;
}
