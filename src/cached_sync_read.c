#include <stdint.h>
#include "headers/_OVERLAPPED.h"

extern void cached_map_issue_async_request(int (*async_fn)(void *, void *, uint32_t, _OVERLAPPED *, void (*)(uint32_t, uint32_t, _OVERLAPPED *)), void *file, _OVERLAPPED *overlapped, void *buffer, uint32_t size, uint32_t offset, void (*completion)(uint32_t, uint32_t, _OVERLAPPED *));
int      ReadFileEx(void *hFile, void *buf, uint32_t size, _OVERLAPPED *overlapped,
                    void (*completion)(uint32_t, uint32_t, _OVERLAPPED *));
extern void cache_file_blocking_io_completion_routine(uint32_t error_code, uint32_t bytes_transferred, _OVERLAPPED *overlapped);
uint32_t SleepEx(uint32_t milliseconds, int alertable);

/* overlapped[0] is the real I/O OVERLAPPED; the two extension slots that follow it
 * are read by cache_file_blocking_io_completion_routine through the same pointer
 * (stack slots var_1C/var_18/var_14 = overlapped+0x24/+0x28/+0x2C in the binary):
 *   overlapped[1].hEvent   = &finished  (the completion signal)
 *   overlapped[2].Internal = optional user callback (0 = none)
 * Declared as one array so the layout the routine indexes into is guaranteed. */
/* return: all 4 callers clrlwi r3,24 after the call (byte-normalization) and the
 * value is an lbz of the finished byte -> declared return is uint8_t, not int */
uint8_t cached_sync_read(void *file, void *buffer, int size, int offset)
{
    volatile uint8_t finished;
    _OVERLAPPED      overlapped[3];

    finished                   = 0;
    overlapped[1].hEvent       = (void *)&finished; /* drops volatile: routine's terminal store */
    overlapped[2].Internal     = 0;
    overlapped[2].InternalHigh = 0;

    cached_map_issue_async_request(ReadFileEx, file, overlapped, buffer, size, offset,
                                   cache_file_blocking_io_completion_routine);
    if (!finished)
    {
        while (SleepEx(0x1388u, 1) == 192 && !finished)
            ;
    }
    return finished;
}
