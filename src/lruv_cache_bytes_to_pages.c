/* lruv_cache_bytes_to_pages @0x8371C4E0 — rounds a byte size up to a page count using the cache's page size. */

#include "headers/lruv_cache.h"

int lruv_cache_bytes_to_pages(lruv_cache *cache, int size_in_bytes)
{
    int page_size_bits = cache->page_size_bits;
    int pages = size_in_bytes >> page_size_bits;
    if (((1 << page_size_bits) - 1) & size_in_bytes)
        ++pages;
    return pages;
}
