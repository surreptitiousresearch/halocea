/* lruv_block_get_address @0x8371C238 — byte address of a cache block's page: its first page index
 * scaled up by the cache's page size in bits. */

#include <stdint.h>

#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

unsigned int lruv_block_get_address(const lruv_cache *cache, int block_index)
{
    /* DEVIATION: block_index is a datum HANDLE (salt<<16 | absolute index); the subscript takes
       only the low word (clrlwi r9,r4,16 @ 0x8371C23C) — same shape as lruv_block_delete. */
    return ((lruv_cache_block *)cache->blocks->data)[(unsigned short)block_index].first_page_index << cache->page_size_bits;
}
