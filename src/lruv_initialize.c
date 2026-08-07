/* lruv_initialize @ 0x8371C048
   Initializes an LRU/validation cache in place: builds the embedded block
   data_array (immediately after the header), validates it, then fills the
   cache header fields. signature 2003133797 marks a live cache; the block list
   starts empty (first/last = -1). */

#include <stdint.h>
#include <string.h>
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

extern void data_initialize(data_array *data, const char *name, int16_t maximum_count, int16_t size);
extern void data_make_valid(data_array *data);
/* strncpy declared by <string.h> */

/* maximum_block_count: extsh r7 @0x8371C068 pins signed 16-bit */
void lruv_initialize(lruv_cache *cache, const char *name, int page_count,
                     int page_size_bits, int16_t maximum_block_count,
                     void (*delete_block_proc)(int),
                     uint8_t (*locked_block_proc)(int))
{
    data_array *blocks = (data_array *)&cache[1]; /* storage after the header */
    data_initialize(blocks, name, maximum_block_count, sizeof(lruv_cache_block));
    data_make_valid(blocks);

    memset(cache, 0, sizeof(lruv_cache));
    strncpy(cache->name, name, 0x1F);
    cache->delete_block_proc = delete_block_proc;
    cache->locked_block_proc = locked_block_proc;
    cache->page_count = page_count;
    cache->page_size_bits = page_size_bits;
    cache->blocks = blocks;
    cache->signature = 0x77656565u; /* 'weee' — live-cache signature (was 2003133797) */
    cache->first_block_index = -1;
    cache->last_block_index = -1;
    cache->frame_index = 1;
}
