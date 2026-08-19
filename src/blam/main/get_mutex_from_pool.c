/* get_mutex_from_pool @0x837FFFF0 — DEVIATION: the decompiler expressed the pool-end test against an
 * adjacent symbol (&vertex_data_3[1].color); mutex_pool is exactly MUTEX_POOL_CAPACITY (32) entries ending
 * at that address, so the scan is reproduced as an index-bounded search (same fix as create_mutex.c's
 * identical pattern). */

#include "headers/mutex.h"
#include "headers/blam_data_globals.h"

#define MUTEX_POOL_CAPACITY 32


mutex * get_mutex_from_pool(void)
{
    int index = 0;

    while ( mutex_pool[index].in_use )
    {
        if ( ++index >= MUTEX_POOL_CAPACITY )
            return 0;
    }

    mutex *slot = &mutex_pool[index];
    slot->object  = 0;
    slot->name[0] = 0;
    slot->in_use  = 1;
    return slot;
}
