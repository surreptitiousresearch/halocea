/* cache_copy_end @0x83807FA8 — stop the background copy worker and block until it finishes: if the copy isn't
 * already complete, signal the stop event then wait for the completion event. */
/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */
#include "headers/cache_copy_worker.h"
#include "headers/blam_data_globals.h"

extern int SetEvent(void *hEvent);
extern unsigned int WaitForSingleObject(void *handle, unsigned int milliseconds);
/* void: r3 at blr is threaded WaitForSingleObject return; the sole caller ignores it. */
void cache_copy_end(void)
{
    if ( WaitForSingleObject(global_self->copy_complete_event, 0) )
    {
        SetEvent(global_self->copy_stop_event);
        WaitForSingleObject(global_self->copy_complete_event, 0xFFFFFFFF);
    }
}
