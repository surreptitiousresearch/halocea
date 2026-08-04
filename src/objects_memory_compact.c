/* objects_memory_compact @0x836F2D38 — run a garbage-collection pass over the object system then compact the
 * object memory pool to reclaim the freed space. */

#include "headers/memory_pool.h"
#include "headers/blam_data_globals.h"


extern void objects_garbage_collection(void);
extern void memory_pool_compact(memory_pool *pool);

void objects_memory_compact(void)
{
    objects_garbage_collection();
    memory_pool_compact(object_memory_pool);
}
