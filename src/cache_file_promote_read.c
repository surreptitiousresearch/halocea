/* cache_file_promote_read @0x83754268 — marks a pending cache-file read request as blocking (promotes it to
 * higher priority). */

#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"


void cache_file_promote_read(int16_t request_index)
{
    cache_file_globals_0.requests[request_index].blocking = 1;
}
