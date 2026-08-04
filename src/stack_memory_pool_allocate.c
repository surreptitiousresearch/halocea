/* stack_memory_pool_allocate @ 0x83803538 — carve a new block (size + 16-byte header, rounded
 * up to 4 bytes) out of a stack memory pool. Tries the tail gap first; if that is too small and
 * the pool is unlocked, compacts the live blocks toward the base to reclaim fragmentation, then
 * searches the inter-block gaps. On success a memory_block is reserved in the block table and
 * linked into the address-ordered block list. Returns null if it can't fit. */

#include "headers/stack_memory_pool.h"

extern void *memmove(void *dst, const void *src, unsigned int n);

memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size)
{
    memory_block *result = 0;
    unsigned int needed;
    unsigned int free_space;
    memory_block *first_block;
    memory_block *gap_predecessor = 0;   /* block after which a found gap sits */
    unsigned char *placement = 0;        /* address chosen for the new block (gap case) */
    memory_block *block;

    if ( size <= 0 )
        return 0;
    if ( (unsigned int)size >= pool->pool_size )
        return 0;

    /* header + payload, rounded up to a 4-byte multiple */
    needed = size + 16;
    while ( needed & 3 )
        ++needed;

    /* free space at the tail (after the last block) */
    free_space = pool->pool_size;
    first_block = pool->first_block;
    if ( first_block )
        free_space += (unsigned int)(&pool->base_address[-(int)MEMORY_BLOCK_SIZE(pool->last_block->bits)]
                                     - (unsigned char *)pool->last_block);
    if ( free_space >= needed )
        goto reserve;

    /* compact live blocks toward the base to reclaim gaps */
    if ( first_block && !pool->locked )
    {
        unsigned char *write_base = pool->base_address;
        int prev_size = 0;
        do
        {
            if ( !MEMORY_BLOCK_IS_IN_USE(first_block->bits)
              && (char *)first_block - prev_size - (char *)write_base > 0 )
            {
                memory_block *moved = (memory_block *)&write_base[prev_size];
                memory_block *prev;
                memmove(&write_base[prev_size], first_block, MEMORY_BLOCK_SIZE(first_block->bits));
                prev = moved->previous_block;
                first_block = moved;
                if ( prev )
                    prev->next_block = moved;
            }
            write_base = (unsigned char *)first_block;
            prev_size = MEMORY_BLOCK_SIZE(first_block->bits);
            first_block = first_block->next_block;
        }
        while ( first_block );
    }

    first_block = pool->first_block;
    free_space = pool->pool_size;
    if ( first_block )
        free_space += (unsigned int)(&pool->base_address[-(int)MEMORY_BLOCK_SIZE(pool->last_block->bits)]
                                     - (unsigned char *)pool->last_block);
    if ( free_space >= needed )
        goto reserve;

    /* search for a gap big enough: before the first block, or between adjacent blocks */
    if ( first_block && (unsigned int)((char *)first_block - (char *)pool->base_address) >= needed )
    {
        placement = pool->base_address;
    }
    else if ( first_block )
    {
        memory_block *cursor = pool->first_block;
        memory_block *next_block = first_block->next_block;
        if ( next_block )
        {
            for ( ;; )
            {
                int cursor_size = MEMORY_BLOCK_SIZE(cursor->bits);
                if ( (unsigned int)((char *)next_block - cursor_size - (char *)cursor) >= needed )
                {
                    placement = (unsigned char *)cursor + cursor_size;
                    gap_predecessor = cursor;
                    break;
                }
                cursor = next_block;
                next_block = next_block->next_block;
                if ( !next_block )
                    break;
            }
        }
    }

    if ( !placement )
        return result;

reserve:
    /* find a free slot in the block table */
    if ( pool->next_block_index == -1 )
    {
        int free_slot = -1;
        if ( pool->number_of_blocks )
        {
            memory_block **slot = pool->blocks;
            int i = 0;
            while ( *slot )
            {
                ++i;
                ++slot;
                if ( i >= pool->number_of_blocks )
                    goto slot_done;
            }
            free_slot = i;
        }
    slot_done:
        pool->next_block_index = free_slot;
    }

    if ( pool->next_block_index == -1 )
        return result;

    /* place the new block at the chosen gap, or append at the tail */
    if ( placement )
        pool->blocks[pool->next_block_index] = (memory_block *)placement;
    else
        pool->blocks[pool->next_block_index] = pool->first_block
            ? (memory_block *)((char *)pool->last_block + MEMORY_BLOCK_SIZE(pool->last_block->bits))
            : (memory_block *)pool->base_address;

    {
        int slot = pool->next_block_index;
        memory_block *nb = pool->blocks[slot];
        nb->bits = needed;
        nb->index = slot;
    }

    result = pool->blocks[pool->next_block_index];

    /* link into the address-ordered doubly linked list */
    if ( pool->first_block )
    {
        if ( result >= pool->first_block )
        {
            memory_block *last = pool->last_block;
            if ( result <= last )
            {
                result->previous_block = gap_predecessor;
                result->next_block = gap_predecessor->next_block;
                gap_predecessor->next_block = result;
                if ( result->next_block )
                    result->next_block->previous_block = result;
            }
            else
            {
                result->previous_block = last;
                result->next_block = 0;
                pool->last_block->next_block = result;
                pool->last_block = result;
            }
        }
        else
        {
            result->previous_block = 0;
            result->next_block = pool->first_block;
            pool->first_block->previous_block = result;
            pool->first_block = result;
        }
    }
    else
    {
        pool->last_block = result;
        pool->first_block = result;
        result->previous_block = 0;
        result->next_block = 0;
    }

    /* advance the free-slot hint past the slot just used */
    {
        int used = pool->next_block_index;
        int n = pool->number_of_blocks;
        int i = used + 1;
        pool->next_block_index = -1;
        if ( i < n )
        {
            memory_block **slot = &pool->blocks[i];
            while ( *slot )
            {
                if ( ++i >= pool->number_of_blocks )
                    return result;
                ++slot;
            }
            pool->next_block_index = i;
        }
    }

    return result;
}
