/* cache_file_windows_thread_wake @0x83753F30 — signals the async I/O thread via Win32 SetEvent. */
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"

extern int SetEvent(void *hEvent);


int cache_file_windows_thread_wake(void)
{
    return SetEvent(cache_file_globals_0.sleep_event);
}
