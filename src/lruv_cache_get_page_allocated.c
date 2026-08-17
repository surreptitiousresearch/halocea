/* lruv_cache_get_page_allocated @0x8371C290 */
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"
#include "headers/data_iterator.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

int lruv_cache_get_page_allocated(lruv_cache *cache)
{
    int total = 0;
    data_iterator iterator;

    data_iterator_new(&iterator, cache->blocks);
    for ( lruv_cache_block *block = data_iterator_next(&iterator); block;
          block = data_iterator_next(&iterator) )
        total += block->page_count;
    return total;
}
