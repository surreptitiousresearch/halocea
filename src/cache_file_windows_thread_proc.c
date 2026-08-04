#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/cache_request.h"
#include "headers/_OVERLAPPED.h"
#include "headers/blam_data_globals.h"

uint32_t WaitForSingleObjectEx(void *handle, uint32_t timeout_ms, int alertable);
extern void * data_file_get_handle(uint32_t data_file_id);
extern void cached_map_issue_async_request(int (*async_fn)(void *, void *, uint32_t, _OVERLAPPED *, void (*)(uint32_t, uint32_t, _OVERLAPPED *)), void *file, _OVERLAPPED *overlapped, void *buffer, uint32_t size, uint32_t offset, void (*completion)(uint32_t, uint32_t, _OVERLAPPED *));
int      ReadFileEx(void *hFile, void *buf, uint32_t size, _OVERLAPPED *overlapped,
                    void (*completion)(uint32_t, uint32_t, _OVERLAPPED *));
extern void cache_file_read_io_completion_routine(uint32_t error_code, uint32_t bytes_transferred, _OVERLAPPED *overlapped);


void cache_file_windows_thread_proc(void)
{
    cache_request *best_request;
    cache_request *candidate;
    void          *file_handle;
    int            i;

    while (1)
    {
        /* 192 == WAIT_IO_COMPLETION: re-wait until the sleep event itself is signalled */
        while (WaitForSingleObjectEx(cache_file_globals_0.sleep_event, 0xFFFFFFFF, 1) == 192)
            ;

        while (1)
        {
            best_request = 0;
            for (i = 0; i < 512; i = (int16_t)(i + 1))
            {
                candidate = &cache_file_globals_0.requests[i];
                if (candidate->pending
                    && !candidate->running
                    && (!best_request
                        || (best_request->blocking > (uint32_t)candidate->blocking
                            && best_request->overlapped.Offset > candidate->overlapped.Offset)))
                {
                    best_request = &cache_file_globals_0.requests[i];
                }
            }

            if (!best_request)
                break;

            file_handle = cache_file_globals_0.cached_map_files[cache_file_globals_0.open_map_file_index].handle;
            if (best_request->data_file)
                file_handle = data_file_get_handle(best_request->data_file);

            best_request->running = 1;
            cached_map_issue_async_request(
                ReadFileEx,
                file_handle,
                &best_request->overlapped,
                best_request->buffer,
                best_request->size,
                best_request->overlapped.Offset,
                cache_file_read_io_completion_routine);
        }
    }
}
