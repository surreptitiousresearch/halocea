#ifndef CACHED_MAP_FILE_PRECACHE_STATE_H
#define CACHED_MAP_FILE_PRECACHE_STATE_H

/* cached_map_file_precache_state — result of cache_files_precache_map_status: the map-precache
 * copy state machine (in progress / finished ok / failed on damaged media). */
enum cached_map_file_precache_state {
	_cached_map_file_in_progress = 0,
	_cached_map_file_success = 1,
	_cached_map_file_failed = 2,
	NUMBER_OF_CACHED_MAP_FILE_PRECACHE_STATES = 3,
};

#endif /* CACHED_MAP_FILE_PRECACHE_STATE_H */

// cached_map_file_precache_state: DB-verified via types_enum_values $C55B709A4DEE68B90C252CCE5E3F6DD9
