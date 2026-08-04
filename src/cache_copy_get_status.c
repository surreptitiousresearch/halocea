/* cache_copy_get_status @0x83808570 — poll the async map-copy worker and report progress. If the copy is
 * blocking, briefly sleep first. Returns: 1 if finished-with-error (flag bit 1), 0/2 derived from the flags
 * when the copy is inactive, 3 if there is nothing to copy (zero-size header), or (copy_complete ? 4 : 3)
 * while a copy runs — in which case *progress is updated (clamped to 0..1) when a progress event is signaled.
 *
 * Deviation: the inactive-path return `(~flags >> 1) & 2` is reproduced verbatim from the decompiler. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_status.h"

/* Win32 imports (kernel32 boundary; system <windows.h> collides with repo Win32 shim types). */
extern void Sleep(unsigned long dwMilliseconds);
extern unsigned long WaitForSingleObject(void *hHandle, unsigned long dwMilliseconds);

int16_t cache_copy_get_status(float *progress)
{
    simple_decompressor_definition *self = global_self;
    volatile unsigned int flags = self->flags;
    if ( self->blocking )
    {
        Sleep(0x10u);
        self = global_self;
    }

    unsigned int result;
    if ( flags || !self->copy_thread )
    {
        if ( (flags & 2) != 0 )
            result = _cache_copy_read_failure;
        else
            /* verbatim: yields _cache_copy_bad_file_failure(0) or _cache_copy_write_failure(2) */
            result = (~flags >> 1) & 2;
        *progress = 0.0f;
    }
    else if ( self->header.size <= 0 )
    {
        *progress = 0.0f;
        return _cache_copy_in_progress;
    }
    else
    {
        /* 3 + copy_complete: _cache_copy_in_progress, or _cache_copy_finised when the copy is done */
        result = (WaitForSingleObject(self->copy_complete_event, 0) == 0) + _cache_copy_in_progress;
        if ( WaitForSingleObject(global_self->progress_update_event, 0) == 0 )
        {
            float read_progress = 0.0f;
            if ( global_self->read_progress >= 0.0f )
            {
                read_progress = 1.0f;
                if ( global_self->read_progress <= 1.0f )
                    read_progress = global_self->read_progress;
            }
            *progress = read_progress;
        }
        return result;
    }
    return result;
}
