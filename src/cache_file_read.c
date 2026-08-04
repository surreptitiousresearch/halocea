/* cache_file_read @0x83754188 — enqueue an async cache read request: find a free (non-pending) request slot
 * (spinning once over all 512 slots if all are busy), fill in the overlapped read parameters, mark it pending,
 * and wake the cache worker thread. Returns the slot index used. */

#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/cache_file_read_request_params.h"
#include "headers/blam_data_globals.h"

/* windows.h dropped: it redefines the repo's _OVERLAPPED/_FILETIME boundary types.
 * Use the sibling-established extern for the only Win32 symbol needed (cache_copy_begin.c pattern). */
extern int SetEvent(void *hEvent);


extern int cache_request_next_free_index(void);
int16_t cache_file_read(int tag_index, uint32_t offset, int size, void *buffer,
                        const cache_file_read_request_params *params, uint8_t blocking, uint8_t data_file)
{
    /* DEVIATION: donor cache_request_next_free_index@0x83753D08 (zero-xref, void params) was inlined verbatim here as the free-slot scan — goto found == donor's implicit return, the write-only 'wrapped' local == donor's dead v0 flag; collapsed to a call, with the request pointer recomputed since the donor only returns the index. */
    int slot = cache_request_next_free_index();
    cache_request *request = &cache_file_globals_0.requests[slot];
    *params->finished_flag = 0;
    request->overlapped.Internal = 0;
    request->overlapped.InternalHigh = 0;
    request->overlapped.Offset = 0;
    request->overlapped.OffsetHigh = 0;
    request->overlapped.hEvent = 0;
    request->running = 0;
    request->size = size;
    request->overlapped.Offset = offset;
    request->buffer = buffer;
    request->pending = 1;
    request->blocking = blocking;
    request->data_file = data_file;
    request->params = *params;
    SetEvent(cache_file_globals_0.sleep_event);
    return slot;
}
