/* cache_file_close @0x837540E8 — close the currently open cached map file: wait for all 512 pending requests
 * to drain, close the file handle, clear its cached_map_file slot, and reset the open-map index.
 *
 * Deviation: the decompiler returns the memset result (void* `result`, uninitialized when no map is open);
 * the engine function is void. */
/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */
#include <string.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"

extern int CloseHandle(void *handle);
extern void Sleep(unsigned int milliseconds);
void cache_file_close(void)
{
    if ( cache_file_globals_0.open_map_file_index != -1 )
    {
        void **p_handle = &cache_file_globals_0.cached_map_files[cache_file_globals_0.open_map_file_index].handle;
        int index = 0;
        do
        {
            cache_request *request = &cache_file_globals_0.requests[index];
            while ( request->pending )
                Sleep(0);
            index = (short)(index + 1);
        }
        while ( index < 512 );
        CloseHandle(*p_handle);
        memset(p_handle, 0, 0x80Cu);
        cache_file_globals_0.open_map_file_index = -1;
    }
}
