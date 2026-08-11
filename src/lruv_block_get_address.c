/* lruv_block_get_address @0x8371C238 — byte address of a cache block's page: its first page index
 * scaled up by the cache's page size in bits. */

#include <stdint.h>

#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

/* block_index attested uint16_t: callee normalizes with clrlwi r4,16 @0x8371C23C */
unsigned int lruv_block_get_address(const lruv_cache *cache, uint16_t block_index)
{
    return ((lruv_cache_block *)cache->blocks->data)[block_index].first_page_index << cache->page_size_bits;
}
