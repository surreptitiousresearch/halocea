/* create_mutex @0x838001E0 — allocate a mutex from the fixed 32-entry mutex pool, give it a unique name
 * ("mutex_<N>"), create the underlying Win32 mutex, and return it through mutex_reference. Returns 1 on
 * success, 0 if the pool is full or the OS mutex could not be created.
 *
 * Deviation: the decompiler expressed the pool-end test against an adjacent symbol (&vertex_data_3[1].color);
 * mutex_pool is exactly MUTEX_POOL_CAPACITY (32) entries ending at that address, so the scan is reproduced as
 * an index-bounded search. */

#include <stdint.h>
#include "headers/mutex.h"
#include "headers/blam_data_globals.h"

#define MUTEX_POOL_CAPACITY 32


extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern void *CreateMutexA(void *security_attributes, int initial_owner, const char *name);

uint8_t create_mutex(mutex **mutex_reference)
{
    mutex *slot = 0;
    int index = 0;

    while ( mutex_pool[index].in_use )
    {
        if ( ++index >= MUTEX_POOL_CAPACITY )
            break;
    }

    if ( index < MUTEX_POOL_CAPACITY )
    {
        slot = &mutex_pool[index];
        slot->object  = 0;
        slot->name[0] = 0;
        slot->in_use  = 1;
    }

    if ( !slot )
    {
        *mutex_reference = 0;
        return 0;
    }

    _snprintf_0(slot->name, 0x20u, "mutex_%ld", mutex_count++);
    slot->object = CreateMutexA(0, 0, slot->name);
    if ( !slot->object )
    {
        *mutex_reference = 0;
        return 0;
    }

    *mutex_reference = slot;
    return 1;
}
