#include "headers/blam_data_globals.h"
/* rasterizer_memory_pool_dispose @0x83791E38 — free the rasterizer scratch memory pool
 * (counterpart of rasterizer_memory_pool_initialize). */

extern void dlFree(void *ptr);
extern void         *global_memory_pool;

void rasterizer_memory_pool_dispose(void)
{
    if (global_memory_pool)
        dlFree(global_memory_pool);

    global_memory_index = 0;
    global_memory_pool   = 0;
}
