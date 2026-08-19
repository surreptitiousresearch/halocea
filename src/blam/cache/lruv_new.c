/* lruv_new @ 0x8371C508
   Allocates an LRU/validation cache: header (68 bytes) plus the block storage
   sized by data_allocation_size(), then initializes it. */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

extern int data_allocation_size(int16_t maximum_count, int16_t size);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void lruv_initialize(lruv_cache *cache, const char *name, int page_count, int page_size_bits, int16_t maximum_block_count, void (*delete_block_proc)(int), uint8_t (*locked_block_proc)(int));

lruv_cache *lruv_new(const char *name, int page_count, int page_size_bits,
                     int16_t maximum_block_count,
                     void (*delete_block_proc)(int),
                     uint8_t (*locked_block_proc)(int))
{
    int block_storage_size = data_allocation_size(maximum_block_count, sizeof(lruv_cache_block));
    lruv_cache *cache = dlMalloc(block_storage_size + sizeof(lruv_cache),
        "D:\\Projects\\code\\HCEX\\sources\\memory\\lruv_cache.c", 0x63);
    if ( cache )
        lruv_initialize(cache, name, page_count, page_size_bits, maximum_block_count,
                        delete_block_proc, locked_block_proc);
    return cache;
}
