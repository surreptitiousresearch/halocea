/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */

#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"
#include "headers/SYSTEMTIME.h"
#include "headers/_FILETIME.h"

/* Win32 boundary function shims (windows.h intentionally excluded) */
extern void __stdcall GetSystemTime(SYSTEMTIME *);
extern int __stdcall SystemTimeToFileTime(const SYSTEMTIME *, _FILETIME *);
extern int __stdcall SetFileTime(void *, const _FILETIME *, const _FILETIME *, const _FILETIME *);

void cached_map_file_set_modification_date(int16_t map_file_index)
{
    cached_map_file *file = &cache_file_globals_0.cached_map_files[map_file_index];
    SYSTEMTIME now;

    GetSystemTime(&now);
    SystemTimeToFileTime(&now, &file->last_modification_date);
    SetFileTime(file->handle, &file->last_modification_date, 0, 0);
}
