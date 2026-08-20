/* cache_copy_initialize @0x83809350 — one-time setup of the async map-copy worker: its four Win32 events
 * (complete: manual-reset, initially signaled; start: auto-reset; stop and progress: manual-reset), the
 * zlib allocator hooks, and the 16KB-stack copy thread. */

/* CAVEAT: dead in the shipped image. This function has ZERO code cross-references --
 * its only reference is the ExceptionDir unwind entry at 0x82370AB8. The DVD-to-HDD copy engine is compiled in
 * and never armed, so a reader should not assume this path runs at runtime.
 * (xrefs to 0x83809350: 1 total, 0 code.) */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/blam_data_globals.h"


/* Declared with zlib's alloc_func/free_func spelling (void*) so they drop straight into the
 * z_stream.zalloc/zfree slots with no cast; the impls carry byte pointers internally. */
extern void *cache_copy_compressed_alloc(void *opaque, unsigned int items, unsigned int size);
extern void cache_copy_compressed_free(void *opaque, void *address);
extern unsigned int simple_cache_copy_thread(void *parameter);

/* Win32 boundary */
extern void *CreateEventA(void *attributes, int manual_reset, int initial_state, const char *name);
extern void *CreateThread(void *attributes, unsigned int stack_size,
        unsigned int (*start_routine)(void *), void *parameter, unsigned int flags,
        unsigned int *thread_id);

void cache_copy_initialize(void)
{
    global_self->copy_complete_event = CreateEventA(0, 1, 1, 0);
    global_self->copy_start_event = CreateEventA(0, 0, 0, 0);
    global_self->copy_stop_event = CreateEventA(0, 1, 0, 0);
    global_self->progress_update_event = CreateEventA(0, 1, 0, 0);

    global_self->zlib_stream.zalloc = cache_copy_compressed_alloc;
    global_self->zlib_stream.zfree = cache_copy_compressed_free;

    global_self->copy_thread = CreateThread(0, 0x4000u, simple_cache_copy_thread, 0, 0, 0);
}
