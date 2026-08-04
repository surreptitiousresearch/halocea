/* tag_block_get_element_with_size @0x83700B20 — address of element `index` in a tag_block given the element
 * stride. */

#include "headers/tag_block.h"

char *tag_block_get_element_with_size(const tag_block *block, int index, int element_size)
{
    return (char *)block->address + index * element_size;
}
