/* cache_copy_begin @0x83807EA8 — kick off an async cache-file copy: if no copy is already running/completing
 * (copy_complete_event not yet signaled), records the destination buffer and source file name, resets the
 * completion/stop events, clears the cached file header, and signals the copy thread to start.
 *
 * DEVIATION: the decompiler rendered the source-name copy as a pointer-difference trick
 * (`v9[v10] = *v9` with v10 = global_self - source_file_name); this is just a plain null-terminated string
 * copy into global_self->src_name, restored as such. destination_file_size is unused, matching the DB's own
 * prototype (never referenced in the decompiled body either). */

/* CAVEAT: dead in the shipped image. This function has ZERO code cross-references --
 * its only reference is the ExceptionDir unwind entry at 0x82370A10. The DVD-to-HDD copy engine is compiled in
 * and never armed, so a reader should not assume this path runs at runtime.
 * (xrefs to 0x83807EA8: 1 total, 0 code.) */

#include <string.h>
#include "headers/simple_decompressor_definition.h"

extern unsigned int WaitForSingleObject(void *handle, unsigned int milliseconds);
extern int ResetEvent(void *hEvent);
extern int SetEvent(void *hEvent);

void cache_copy_begin(void *buffer, int size, void *destination_file, int destination_file_size, const char *source_file_name)
{
    if ( WaitForSingleObject(global_self->copy_complete_event, 0) )
        return;

    global_self->allocated_buffer = buffer;
    global_self->flags = 0;

    char *dst = global_self->src_name;
    while ( (*dst++ = *source_file_name++) != 0 )
        ;

    global_self->write_file_handle = destination_file;
    global_self->read_progress = 0.0f;
    ResetEvent(global_self->copy_complete_event);
    ResetEvent(global_self->copy_stop_event);
    memset(&global_self->header, 0, sizeof(global_self->header));
    SetEvent(global_self->copy_start_event);
}
