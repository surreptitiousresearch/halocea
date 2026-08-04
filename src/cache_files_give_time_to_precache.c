/* cache_files_give_time_to_precache @0x836F71F0 — service the map precache state machine for map_name:
 * returns 1 if already loaded; otherwise ends a precache of a different map, advances/finishes the current
 * precache, or starts a new low-priority precache. On damaged media raises a fatal damaged-media error (when
 * fatal) or defers error 35. Returns 1 only when the map is fully precached.
 *
 * Deviation: v6 is the precache-status-out scratch float required by cache_files_precache_map_status. */

#include <stdint.h>
#include "headers/error_type.h"
#include "headers/cached_map_file_precache_state.h"

extern uint8_t cache_files_precache_map_loaded(const char *name);
extern uint8_t cache_files_precache_in_progress(void);
extern uint8_t cache_files_precache_is_copying_map(const char *name);
extern void cache_files_precache_map_end(void);
extern int16_t cache_files_precache_map_status(float *progress);
extern void cache_files_precache_set_priority(uint8_t blocking);
extern uint8_t cache_files_precache_map_begin(const char *name, uint8_t blocking);
extern void display_error_damaged_media(void);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

int cache_files_give_time_to_precache(const char *map_name, uint8_t fatal)
{
    if ( cache_files_precache_map_loaded(map_name) )
        return 1;

    if ( cache_files_precache_in_progress() && !cache_files_precache_is_copying_map(map_name) )
        cache_files_precache_map_end();

    if ( cache_files_precache_in_progress() )
    {
        float status;
        __int16 precache_status = cache_files_precache_map_status(&status);
        if ( precache_status == _cached_map_file_failed )
        {
            if ( fatal )
            {
                display_error_damaged_media();
                return 0;
            }
            display_error_deferred(_error_media_damaged, 0, 0, 0);
            return 0;
        }
        if ( precache_status == _cached_map_file_success )
        {
            cache_files_precache_map_end();
            return 0;
        }
    }
    else
    {
        cache_files_precache_set_priority(0);
        if ( !cache_files_precache_map_begin(map_name, 0) )
        {
            if ( fatal )
            {
                display_error_damaged_media();
                return 0;
            }
            display_error_deferred(_error_media_damaged, 0, 0, 0);
        }
    }

    return 0;
}
