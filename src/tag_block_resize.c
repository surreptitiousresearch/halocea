/* tag_block_resize @0x836F7058 — no-op in the cache (release) build: tag blocks are fixed at cache-build
 * time and cannot be resized at runtime. Always returns 0 (failure). */

#include <stdint.h>
#include "headers/tag_block.h"

/* attested: uint8_t return — callers byte-normalize (clrlwi r11, r3, 24 @ 83730024 et al.) */
uint8_t tag_block_resize(tag_block *block, int element_count)
{
    return 0;
}
