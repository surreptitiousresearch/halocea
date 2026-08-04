/* main_queue_map_private @0x836893A8 — drive the queued-map precache state machine: finish a completed
 * precache, and when neither a precache nor a Bink movie is active, begin precaching the queued map.
 *
 * Deviation: database types this int; the values are ABI residue from the predicate calls. Written as void.
 * v1[4] is the precache status-out scratch buffer required by cache_files_precache_map_status. */

#include <stdint.h>
#include "headers/main_globals.h"

extern uint8_t cache_files_precache_in_progress(void);
extern int16_t cache_files_precache_map_status(float *progress);
extern void cache_files_precache_map_end(void);
extern uint8_t bink_playback_active(void);
extern uint8_t cache_files_precache_map_begin(const char *name, uint8_t blocking);

void main_queue_map_private(void)
{
    float status[4];
    if ( cache_files_precache_in_progress() && cache_files_precache_map_status(status) == 1 )
        cache_files_precache_map_end();

    if ( !cache_files_precache_in_progress() && !bink_playback_active() )
    {
        cache_files_precache_map_begin(main_globals.queued_map_name, 0);
        main_globals.queue_map = 0;
    }
}
