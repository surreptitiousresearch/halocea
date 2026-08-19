/* cache_files_precache_set_priority @0x83753DD0 — set the async map-copy worker's priority (thunk to
 * cache_copy_set_priority). */

#include <stdint.h>

extern void cache_copy_set_priority(uint8_t blocking);

void cache_files_precache_set_priority(uint8_t blocking)
{
    cache_copy_set_priority(blocking);
}
