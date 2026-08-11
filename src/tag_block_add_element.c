/* tag_block_add_element @0x836F7068 — add an element to a tag block. In this (cache-backed runtime) build tag
 * blocks are read-only, so the operation always fails (returns -1, the no-element sentinel). */

#include <stdint.h>
#include "headers/tag_groups.h"

/* attested: int16_t return (element index) — callers re-extsh r3 (extsh @ 8372EBB4 et al.) */
int16_t tag_block_add_element(tag_block *block)
{
    return -1;
}
