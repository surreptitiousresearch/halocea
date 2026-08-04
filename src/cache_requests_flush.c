/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */

#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"
extern void Sleep(unsigned int milliseconds);


void cache_requests_flush(void)
{
    for ( int i = 0; i < 512; i++ )
    {
        cache_request *request = &cache_file_globals_0.requests[i];

        while ( request->pending )
            Sleep(0);
    }
}
