/* cached_map_file_read_header @0x83754A38 — read the 2048-byte header of a cached map file slot via an async
 * (ReadFileEx + alertable wait) request, stamp its last-modification time, and verify it. On verify failure
 * the header is zeroed; if the read never completed the slot's handle is invalidated. */

#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/async_read_request.h"
#include "headers/blam_data_globals.h"
#include "headers/cache_file_header.h"
extern unsigned int SleepEx(unsigned int dwMilliseconds, int bAlertable);

/* Win32 imports (kernel32 boundary; repo shim types — system <windows.h> collides with the
 * canonical globals header's Win32 shims) */
extern int GetFileTime(void *hFile, _FILETIME *lpCreationTime, _FILETIME *lpLastAccessTime, _FILETIME *lpLastWriteTime);
/* CAST-FIX: lpCompletionRoutine is a completion-routine funcptr (Win32 LPOVERLAPPED_COMPLETION_ROUTINE),
 * not void*; typed so ReadFileEx matches cached_map_issue_async_request's async_fn param with no cast. */
extern int ReadFileEx(void *hFile, void *lpBuffer, uint32_t nNumberOfBytesToRead, _OVERLAPPED *lpOverlapped, void (*lpCompletionRoutine)(uint32_t, uint32_t, _OVERLAPPED *));

extern char *system_get_scratch_disk_root(void);
extern int sprintf_0(char *string, const char *format, ...);
extern void cached_map_issue_async_request(int (*async_fn)(void *, void *, uint32_t, _OVERLAPPED *, void (*)(uint32_t, uint32_t, _OVERLAPPED *)), void *file, _OVERLAPPED *overlapped, void *buffer, uint32_t size, uint32_t offset, void (*completion)(uint32_t, uint32_t, _OVERLAPPED *));
extern void cache_file_blocking_io_completion_routine(uint32_t error_code, uint32_t bytes_transferred, _OVERLAPPED *overlapped);
extern uint8_t cache_file_header_verify(const cache_file_header *header, const char *name, uint8_t fatal);
extern void *memset(void *dst, int c, unsigned int n);

void cached_map_file_read_header(int16_t map_file_index)
{
    cached_map_file *file = &cache_file_globals_0.cached_map_files[map_file_index];
    const char *scratch_disk_root = system_get_scratch_disk_root();
    char path[304];
    sprintf_0(path, "%s\\cache%03d.map", scratch_disk_root, (int)map_file_index);

    GetFileTime(file->handle, &file->last_modification_date /* repo shim spelling */, 0, 0);

    async_read_request request;  /* OVERLAPPED + completion-flag ptr; see headers/async_read_request.h */
    volatile char completed = 0;
    request.completed_flag = &completed;
    request.unused1 = 0;
    request.unused2 = 0;

    cached_map_issue_async_request(ReadFileEx, file->handle, &request.overlapped, &file->header, 2048, 0,
                                   cache_file_blocking_io_completion_routine);
    if ( !completed )
    {
        while ( SleepEx(0x1388u, 1) == 192 && !completed )
            ;
    }

    if ( completed )
    {
        if ( !cache_file_header_verify(&file->header, path, 0) )
        {
            memset(&file->header, 0, sizeof(file->header));
            file->last_modification_date.dwHighDateTime = 0;
            file->last_modification_date.dwLowDateTime = 0;
        }
    }
    else
    {
        file->handle = (void *)-1;
    }
}
