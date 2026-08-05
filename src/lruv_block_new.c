/* lruv_block_new @ 0x8371C668 — allocate a run of pages from an LRU virtual block cache. Walks
 * the blocks in page order maintaining a set of candidate windows (start position + accumulated
 * free/evictable pages); a window that reaches the requested page count is a candidate, scored by
 * the newest frame it would have to evict (older = better). Locked blocks and blocks touched this
 * frame reset the windows. The best candidate's overlapping blocks (and the LRU victim, if the
 * cache is full) are evicted, then a new block datum is linked in at the chosen page offset.
 * Returns the new block index, or -1 if no run is large enough.
 *
 * Block record = 7 dwords (28 bytes): [1]=size_in_pages, [2]=page_offset, [3]=next, [4]=previous,
 * [5]=last_touched_frame. Faithful reproduction including the original's 255-wrap window ring. */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"
#include "headers/data_iterator.h"

extern int datum_new(data_array *data);
extern void lruv_block_delete(lruv_cache *cache, uint16_t block_index);

#include "headers/lruv_window.h"

int lruv_block_new(lruv_cache *cache, int size)
{
    int page_size_bits = cache->page_size_bits;
    int pages_needed = size >> page_size_bits;
    int cur_block;
    char found = 0;
    int victim = -1;
    int16_t window_head = 0;
    int16_t window_tail = 0;
    int prev_block = -1;
    int page_cursor = 0;
    unsigned int best_victim_frame = 0;
    unsigned int best_frame = 0;
    int chosen_start_block = -1;
    int chosen_page_offset = 0;
    int chosen_accumulated = 0;
    lruv_window windows[8];
    int insert_after;
    int result;
    lruv_cache_block *blocks = (lruv_cache_block *)cache->blocks->data;
    lruv_cache_block *block;
    lruv_cache_block *iter_block;
    data_iterator it;

    if ( ((1 << page_size_bits) - 1) & size )
        ++pages_needed;

    cur_block = cache->first_block_index;

    if ( cache->page_count > 0 )
    {
        do
        {
            unsigned int run_frame;
            int run_pages;
            char blocking;
            int page_count_here;

            /* open a new candidate window at the current position (if the ring isn't full) */
            {
                int16_t next_tail = (window_tail != 255) ? (int16_t)(window_tail + 1) : 0;
                if ( next_tail != window_head )
                {
                    lruv_window *w = &windows[window_tail];
                    w->start_block = prev_block;
                    w->page_offset = page_cursor;
                    w->max_frame = 0;
                    w->accumulated_pages = 0;
                    window_tail = (window_tail != 255) ? (int16_t)(window_tail + 1) : 0;
                }
            }

            if ( cur_block == -1 )
            {
                page_count_here = cache->page_count;   /* trailing gap to the end */
            }
            else
            {
                block = &blocks[(uint16_t)cur_block];
                if ( page_cursor == block->first_page_index )   /* adjacent allocated block (no gap) */
                {
                    unsigned char (*locked_proc)(int) = cache->locked_block_proc;
                    run_frame = block->last_used_frame_index;
                    run_pages = block->page_count;
                    blocking = (locked_proc && locked_proc(cur_block)) ? 1 : 0;
                    if ( block->last_used_frame_index == cache->frame_index )
                        blocking = 1;
                    if ( !blocking && (victim == -1 || block->last_used_frame_index < best_victim_frame) )
                    {
                        victim = cur_block;
                        best_victim_frame = block->last_used_frame_index;
                    }
                    prev_block = cur_block;
                    cur_block = block->next_block_index;
                    page_cursor = block->first_page_index + block->page_count;
                    goto extend_windows;
                }
                page_count_here = block->first_page_index;   /* gap before this block */
            }

            /* a free gap: fully evictable */
            run_pages = page_count_here - page_cursor;
            run_frame = 0;
            page_cursor = page_count_here;
            blocking = 0;

        extend_windows:
            if ( blocking )
            {
                window_head = window_tail;   /* a locked/in-use block invalidates all open windows */
            }
            else if ( window_head != window_tail )
            {
                int k = window_head;
                do
                {
                    lruv_window *w = &windows[k];
                    if ( run_frame > w->max_frame )
                        w->max_frame = run_frame;
                    w->accumulated_pages += run_pages;
                    if ( w->accumulated_pages >= pages_needed )
                    {
                        if ( !found
                          || w->max_frame < best_frame
                          || (w->max_frame == best_frame && w->accumulated_pages < chosen_accumulated) )
                        {
                            found = 1;
                            chosen_start_block = w->start_block;
                            chosen_page_offset = w->page_offset;
                            chosen_accumulated = w->accumulated_pages;
                            best_frame = w->max_frame;
                        }
                        window_head = (window_head != 255) ? (int16_t)(window_head + 1) : 0;
                    }
                    k = (k != 255) ? (int16_t)(k + 1) : 0;
                }
                while ( k != window_tail );
            }
        }
        while ( page_cursor < cache->page_count );
    }

    if ( !found )
        return -1;

    /* evict every block overlapping the chosen run */
    data_iterator_new(&it, cache->blocks);
    for ( iter_block = data_iterator_next(&it); iter_block;
          iter_block = data_iterator_next(&it) )
    {
        int offset = iter_block->first_page_index;
        if ( offset < pages_needed + chosen_page_offset && iter_block->page_count + offset > chosen_page_offset )
            lruv_block_delete(cache, it.index);
    }

    /* if the cache is full, drop the LRU victim too (re-anchor the insert point if needed) */
    insert_after = chosen_start_block;
    {
        data_array *block_array = cache->blocks;
        if ( block_array->actual_count == block_array->maximum_count && victim != -1 )
        {
            if ( chosen_start_block == victim )
                insert_after = blocks[(uint16_t)victim].previous_block_index; /* victim's prev */
            lruv_block_delete(cache, victim);
        }
    }

    /* link the new block into the page-ordered list */
    result = datum_new(cache->blocks);
    if ( result != -1 )
    {
        lruv_cache_block *nb = &blocks[(uint16_t)result];
        if ( insert_after == -1 )
        {
            int first = cache->first_block_index;
            nb->previous_block_index = -1;
            if ( first == -1 )
                cache->last_block_index = result;
            else
                blocks[(uint16_t)first].previous_block_index = result;
            nb->next_block_index = cache->first_block_index;
            cache->first_block_index = result;
        }
        else
        {
            lruv_cache_block *after = &blocks[(uint16_t)insert_after];
            if ( after->next_block_index == -1 )
            {
                nb->previous_block_index = cache->last_block_index;
                cache->last_block_index = result;
            }
            else
            {
                lruv_cache_block *next = &blocks[(uint16_t)after->next_block_index];
                nb->previous_block_index = next->previous_block_index;
                next->previous_block_index = result;
            }
            nb->next_block_index = after->next_block_index;
            after->next_block_index = result;
        }
        nb->first_page_index = chosen_page_offset;
        nb->page_count = pages_needed;
        nb->last_used_frame_index = cache->frame_index;
    }
    return result;
}
