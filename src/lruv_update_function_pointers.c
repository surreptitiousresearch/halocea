#include "headers/lruv_cache.h"

void lruv_update_function_pointers(lruv_cache *cache,
                                   void (*delete_block_proc)(int),
                                   unsigned char (*locked_block_proc)(int))
{
    cache->delete_block_proc = delete_block_proc;
    cache->locked_block_proc = locked_block_proc;
}
