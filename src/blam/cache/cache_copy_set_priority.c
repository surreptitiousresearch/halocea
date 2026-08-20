/* cache_copy_set_priority @0x83807E40 — record the blocking flag and set the async map-copy worker
 * thread's priority from it: priority 1 (above normal) when blocking, priority 0 (normal) when not.
 * The previous banner had this inverted. Proof: clrlwi r10,r3,24 / cmplwi cr6,r10,0 @0x83807E44-0x83807E48,
 * beq -> loc_83807E64 @0x83807E58 where li r4,0 @0x83807E64; the fall-through at 0x83807E5C is li r4,1.
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
