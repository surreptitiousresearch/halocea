/* memory_block_get_user_size @0x83803118 */
#include "headers/memory_block.h"

unsigned int memory_block_get_user_size(memory_block *block)
{
    return (block->bits & 0x7FFFFFFFu) - 16;
}
