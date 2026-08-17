/* cache_files_precache_map_status @0x83754880 — translate the low-level cache-copy status (with progress) into
 * the precache status code the caller expects: 0/1/2 copy-in-progress -> 2, 3 -> 0 (done), 4 -> 1. When the
 * copy reports "finished" (status 2) the destination cached-map-file handle is invalidated.
 *
 * CAVEAT: as-shipped — for an out-of-range status (>4) the binary returns an uninitialized stack halfword
 * (lhz r3, 0x60+var_10(r1) @0x83754920; no store to that slot in the function); reproduced verbatim. The
 * redundant `&& status` guards the decompiler emitted are dropped as no-ops. */

#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/cache_copy_status.h"
#include "headers/cached_map_file_precache_state.h"
#include "headers/blam_data_globals.h"

extern int16_t cache_copy_get_status(float *progress);

int16_t cache_files_precache_map_status(float *progress)
{
    uint16_t uninitialized;   /* as-shipped: returned on the out-of-range path (lhz @0x83754920) */
    int16_t status = cache_copy_get_status(progress);

    if ( (unsigned int)status >= NUMBER_OF_CACHE_COPY_STATES )
        return uninitialized;
    if ( status == _cache_copy_read_failure )
        return _cached_map_file_failed;
    if ( status == _cache_copy_write_failure )
    {
        cache_file_globals_0.cached_map_files[cache_file_globals_0.copying_to_map_file_index].handle = (void *)-1;
        return _cached_map_file_failed;
    }
    if ( status == _cache_copy_in_progress )
        return _cached_map_file_in_progress;
    if ( status == _cache_copy_bad_file_failure )
        return _cached_map_file_failed;
    return _cached_map_file_success;   /* _cache_copy_finised */
}
