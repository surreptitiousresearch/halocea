/* cache_copy_queue_end @0x83807F58 — request the background copy worker to stop (non-blocking): if the copy
 * isn't already complete, signal its stop event. */
/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */
#include "headers/cache_copy_worker.h"
#include "headers/blam_data_globals.h"

extern int SetEvent(void *hEvent);
extern unsigned int WaitForSingleObject(void *handle, unsigned int milliseconds);
/* void: r3 at blr is threaded callee residue (SetEvent / WaitForSingleObject return);
 * no callers consume it (0 sampled). */
void cache_copy_queue_end(void)
{
    if ( WaitForSingleObject(global_self->copy_complete_event, 0) )
        SetEvent(global_self->copy_stop_event);
}
