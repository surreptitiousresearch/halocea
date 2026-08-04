#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/cache_request.h"
#include "headers/blam_data_globals.h"


cache_request * cache_request_get(int16_t request_index)
{
    return &cache_file_globals_0.requests[request_index];
}
