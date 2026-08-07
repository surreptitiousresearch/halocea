/* lruv_cache_get_page_usage @ 0x8371C2F8 — fill a per-page state bitmap for the LRU virtual cache.
 * Every page starts at 0; each live block stamps its page span (offset i[2], length i[1]) with a
 * state byte: bit0 always set (allocated), bit3 set if the block is currently locked, bit1 set if it
 * was touched this frame (last_used_frame_index == cache frame), bit2 set if it is old
 * (last_used_frame_index + 30 < cache frame). */

#include <string.h>
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"
#include "headers/data_iterator.h"


void lruv_cache_get_page_usage(lruv_cache *cache, unsigned char *page_usage)
{
    data_iterator iterator;
    lruv_cache_block *block;

    memset(page_usage, 0, cache->page_count);
    data_iterator_new(&iterator, cache->blocks);
    for ( block = data_iterator_next(&iterator); block;
          block = data_iterator_next(&iterator) )
    {
        unsigned char (*locked_block_proc)(int) = cache->locked_block_proc;
        unsigned char state = 1;                       /* allocated */
        unsigned int block_frame;
        unsigned int frame_index;

        if ( locked_block_proc && locked_block_proc(iterator.index) )
            state = 9;                                 /* allocated | locked */
        block_frame = block->last_used_frame_index;
        frame_index = cache->frame_index;
        if ( block_frame == frame_index )
            state |= 2u;                               /* used this frame */
        if ( block_frame + 30 < frame_index )
            state |= 4u;                               /* old */
        memset(&page_usage[block->first_page_index], state, block->page_count);
    }
}
