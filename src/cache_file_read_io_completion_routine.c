/* cache_file_read_io_completion_routine @0x83753F80 */
#include <stdint.h>
#include "headers/_OVERLAPPED.h"
#include "headers/cache_request.h"

void cache_file_read_io_completion_routine(uint32_t error_code, uint32_t bytes_transferred,
                                           _OVERLAPPED *overlapped)
{
    cache_request *req = (cache_request *)overlapped;
    *req->params.finished_flag = 1;
    req->pending               = 0;
    req->running               = 0;
}
