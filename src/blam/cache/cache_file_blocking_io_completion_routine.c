/* cache_file_blocking_io_completion_routine @0x83753FA0 */
#include <stdint.h>
#include "headers/_OVERLAPPED.h"

/* overlapped extension layout (stack in cached_sync_read):
 *   overlapped[0] : _OVERLAPPED (the actual I/O overlapped)
 *   overlapped[1].hEvent   : finished_flag pointer (uint8_t *)
 *   overlapped[2].Internal : optional user callback  */

void cache_file_blocking_io_completion_routine(uint32_t error_code, uint32_t bytes_transferred,
                                               _OVERLAPPED *overlapped)
{
    void (*user_callback)(void **, uint32_t);

    user_callback = (void (*)(void **, uint32_t))overlapped[2].Internal;
    if (user_callback)
        user_callback(&overlapped[1].hEvent, bytes_transferred);
    *(uint8_t *)overlapped[1].hEvent = 1;
}
