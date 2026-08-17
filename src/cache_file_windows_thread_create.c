/* cache_file_windows_thread_create @0x83755078 */
/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */

#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"

/* Win32 boundary externs, spelled exactly as in cache_files_initialize.c — the other TU that
 * starts this same thread. They previously differed here (__stdcall, a local
 * LPTHREAD_START_ROUTINE typedef, unsigned long *thread_id), which made one interface look like
 * two. Xbox 360 has no __stdcall, so the qualifier was noise. */
void *CreateEventA(void *attributes, int manual_reset, int initial_state, const char *name);
void *CreateThread(void *attributes, uint32_t stack_size,
                   uint32_t (*start)(void *), void *param,
                   uint32_t flags, uint32_t *thread_id);

extern void cache_file_windows_thread_proc(void);

/* CreateThread's handle is left in r3 at the blr after being stored to cache_file_globals_0.thread,
 * but no caller consumes it — attested void. */
void cache_file_windows_thread_create(void)
{
    cache_file_globals_0.sleep_event = CreateEventA(0, 0, 0, 0);

    cache_file_globals_0.thread = CreateThread(
        0,
        0x4000u,
        /* faithful Win32 boundary pun: the real proc is void(void) (never returns — it tail-branches
         * back into its own wait loop at 0x83754D44 — and reads no argument register); cast to the
         * LPTHREAD_START_ROUTINE shape the API requires, as in the binary. */
        (uint32_t (*)(void *))cache_file_windows_thread_proc,
        0,
        0,
        0);
}
