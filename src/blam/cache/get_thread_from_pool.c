/* get_thread_from_pool @0x837FFF98 — allocate a slot from the fixed 32-entry thread pool. Returns null if
 * the pool is full.
 *
 * Deviation: the decompiler expressed the pool-end test against an adjacent symbol (mutex_pool[0].name);
 * thread_pool is exactly THREAD_POOL_CAPACITY (32) entries ending where mutex_pool begins, so the scan is
 * reproduced as an index-bounded search (same pattern as create_mutex.c's mutex_pool scan). */

#include "headers/thread.h"
#include "headers/blam_data_globals.h"

#define THREAD_POOL_CAPACITY 32


thread * get_thread_from_pool(void)
{
    int index = 0;

    while (thread_pool[index].in_use)
    {
        if (++index >= THREAD_POOL_CAPACITY)
            return nullptr;
    }

    thread *slot = &thread_pool[index];
    slot->object = nullptr;
    slot->in_use = 1;
    return slot;
}
