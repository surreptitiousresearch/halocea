/* cache_copy_set_priority @0x83807E40 — set the async map-copy worker thread's priority: above-normal (1)
 * while non-blocking precaching, normal (0) when blocking.
 *
 * Boundary: SetThreadPriority is a Win32 extern. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/blam_data_globals.h"

extern int SetThreadPriority(void *thread, int priority);

void cache_copy_set_priority(uint8_t blocking)
{
    global_self->blocking = blocking;
    if ( blocking == 0 )
        SetThreadPriority(global_self->copy_thread, 0);
    else
        SetThreadPriority(global_self->copy_thread, 1);
}
