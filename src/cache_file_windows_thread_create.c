/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */

#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"

/* minimal Win32 shim: windows.h intentionally excluded (see header note above) */
typedef unsigned long(__stdcall *LPTHREAD_START_ROUTINE)(void *);
extern void *__stdcall CreateEventA(void *, int, int, const char *);
extern void *__stdcall CreateThread(void *, unsigned int, LPTHREAD_START_ROUTINE, void *, unsigned int, unsigned long *);

extern void cache_file_windows_thread_proc(void);

/* CreateThread's handle is left in r3 at the blr after being stored to cache_file_globals_0.thread,
 * but no caller consumes it — attested void. */
void cache_file_windows_thread_create(void)
{
    cache_file_globals_0.sleep_event = CreateEventA(0, 0, 0, 0);

    cache_file_globals_0.thread =
        CreateThread(0, 0x4000u, (LPTHREAD_START_ROUTINE)cache_file_windows_thread_proc, 0, 0, 0);
}
