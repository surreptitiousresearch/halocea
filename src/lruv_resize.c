/* lruv_resize @0x8371C3C8 — shrink (or grow) an LRU/validation cache to `new_page_count` pages: evicts
 * every block whose page range (page_index + page_count) no longer fits, unlinking it from the
 * first/last-block doubly-linked list (block record: 28 bytes, next @+12, prev @+16) and deleting its
 * datum. Finally updates the cache's page_count.
 *
 * Disasm-verified: the delete_block_proc callback is invoked with r3 unchanged from the function's own
 * `cache` parameter (no explicit argument setup at the call site) — reproduced as-is, since no block index
 * has been computed yet at that point in the compiled code. */

#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"
#include "headers/data_iterator.h"

#include "headers/data_array.h"
extern void datum_delete(data_array *data, int index);

void lruv_resize(lruv_cache *cache, int new_page_count)
{
    data_iterator iterator;
    data_iterator_new(&iterator, cache->blocks);

    lruv_cache_block *blocks = (lruv_cache_block *)cache->blocks->data;
    for ( lruv_cache_block *block = data_iterator_next(&iterator); block; block = data_iterator_next(&iterator) )
    {
        if ( block->first_page_index + block->page_count > new_page_count )
        {
            if ( cache->delete_block_proc )
                ((void (*)(int))cache->delete_block_proc)((int)cache);

            int index = iterator.index;
            lruv_cache_block *record = &blocks[(unsigned __int16)iterator.index];

            /* decompiler local "next_index" is actually the previous link (record+4=+0x10) */
            int previous_index = record->previous_block_index;
            if ( previous_index == -1 )
                cache->first_block_index = record->next_block_index;
            else
                blocks[(unsigned __int16)previous_index].next_block_index = record->next_block_index;

            int next_index = record->next_block_index;
            if ( next_index == -1 )
                cache->last_block_index = record->previous_block_index;
            else
                blocks[(unsigned __int16)next_index].previous_block_index = record->previous_block_index;

            datum_delete(cache->blocks, index);
        }
    }

    cache->page_count = new_page_count;
}
