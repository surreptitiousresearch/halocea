/* cache_files_precache_map_end @0x83755000 — finish a background map copy: end the copy, return the texture
 * cache memory, stamp the freshly copied file with the current time, re-read its header, and clear the
 * in-progress copy state. */

/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */
#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"
#include "headers/SYSTEMTIME.h"
#include "headers/_FILETIME.h"

/* Win32 boundary function shims (windows.h intentionally excluded) */
extern void __stdcall GetSystemTime(SYSTEMTIME *);
extern int __stdcall SystemTimeToFileTime(const SYSTEMTIME *, FILETIME *);
extern int __stdcall SetFileTime(void *, const FILETIME *, const FILETIME *, const FILETIME *);

extern void cache_copy_end(void);
extern void texture_cache_return_memory(void);
extern void cached_map_file_read_header(int16_t map_file_index);

void cache_files_precache_map_end(void)
{
    cache_copy_end(); /* attested void callee: r3-threading removed, call order preserved */
    texture_cache_return_memory();

    cached_map_file *file = &cache_file_globals_0.cached_map_files[cache_file_globals_0.copying_to_map_file_index];
    SYSTEMTIME now;
    GetSystemTime(&now);
    SystemTimeToFileTime(&now, &file->last_modification_date);
    SetFileTime(file->handle, &file->last_modification_date, 0, 0);

    cached_map_file_read_header(cache_file_globals_0.copying_to_map_file_index);
    cache_file_globals_0.copy_in_progress = 0;
    cache_file_globals_0.copying_to_map_file_index = -1;
}
