/* memory_block_valid @0x83803510 — false for a null block or one whose size field reads back as the
 * sentinel value 16 (matches sizeof(memory_block) itself — the header-only, no-payload marker). */

#include <stdint.h>
#include "headers/memory_block.h"

uint8_t memory_block_valid(memory_block *block)
{
    if (!block)
        return 0;
    return (block->bits & 0x7FFFFFFF) != 0x10;
}
