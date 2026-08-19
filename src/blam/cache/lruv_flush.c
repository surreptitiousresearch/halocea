/* lruv_flush @0x8371C580 — evict every block from an LRU/validation cache: for each live block, invoke the
 * cache's delete-block callback, unlink it from the doubly-linked LRU list (fixing up the neighbours' prev/next
 * or the cache head/tail), and delete the datum. */

#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"
#include "headers/data_iterator.h"

#include "headers/data_array.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void datum_delete(data_array *data, int index);

void lruv_flush(lruv_cache *cache)
{
    data_iterator iterator;
    data_iterator_new(&iterator, cache->blocks);
    while ( data_iterator_next(&iterator) )
    {
        void (*delete_block_proc)(int) = cache->delete_block_proc;
        int index = iterator.index;
        lruv_cache_block *blocks = (lruv_cache_block *)cache->blocks->data;
        lruv_cache_block *block = &blocks[(unsigned short)iterator.index];

        if ( delete_block_proc )
            delete_block_proc(index);   /* disasm 0x8371C5D8: r3=index at bctrl; the block index is the callback arg */

        /* unlink from the LRU list */
        if ( block->previous_block_index == -1 )
            cache->first_block_index = block->next_block_index;
        else
            blocks[(unsigned short)block->previous_block_index].next_block_index = block->next_block_index;

        if ( block->next_block_index == -1 )
            cache->last_block_index = block->previous_block_index;
        else
            blocks[(unsigned short)block->next_block_index].previous_block_index = block->previous_block_index;

        datum_delete(cache->blocks, index);
    }
}
