/* tag_block_get_element_with_size @0x83700B20 — address of element `index` in a tag_block given the element
 * stride. */

#include <stdint.h>
#include "headers/tag_groups.h"

/* DEVIATION: returns `void *` per the DB decl @0x83700B20; the pre-B12b-2 spelling was `char *`.
 * The binary cannot tell the two apart, so the DB decl decides. */
void *tag_block_get_element_with_size(const tag_block *block, int index, int element_size)
{
    return (char *)block->address + index * element_size;
}
