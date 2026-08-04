/* game_precache_new_map @0x836878F0 — kick off (or finish) precaching of the named map's cache file.
 * In non-blocking mode it queues the map name and lets precache continue in the background; in blocking mode
 * it finishes any in-progress precache, persists the solo map, clears the queue, and (for single-player)
 * remembers player 1's profile. */

#include <stdint.h>
#include "headers/game_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t cache_files_precache_map_loaded(const char *name);
extern uint8_t cache_files_precache_in_progress(void);
extern uint8_t cache_files_precache_is_copying_map(const char *name);
extern void cache_files_precache_map_end(void);
extern void cache_files_precache_map_queue_end(void);
extern uint8_t cache_files_precache_map_begin(const char *name, uint8_t blocking);
extern void main_queue_map_name(const char *new_name);
extern void main_save_current_solo_map(const char *map_name);
extern void player_ui_remember_player1_profile(uint8_t write_to_disk);

void game_precache_new_map(const char *map_name, uint8_t blocking)
{
    if ( !cache_files_precache_map_loaded(map_name) )
    {
        if ( cache_files_precache_in_progress() && !cache_files_precache_is_copying_map(map_name) )
        {
            if ( blocking )
            {
                cache_files_precache_map_end();
            }
            else
            {
                cache_files_precache_map_queue_end();
                main_queue_map_name(map_name);
            }
        }
        if ( !cache_files_precache_in_progress() )
            cache_files_precache_map_begin(map_name, blocking);
        if ( blocking )
        {
            game_globals->loading_progress = 1.0;
            game_globals->map_load_in_progress = 0;
        }
    }

    if ( blocking )
    {
        main_save_current_solo_map(map_name);
        main_queue_map_name(nullptr); /* clear the queued map name */
        if ( cache_files_precache_in_progress() )
            cache_files_precache_map_end();
        if ( player_spawn_count == 1 )
            player_ui_remember_player1_profile(1u);
    }
}
