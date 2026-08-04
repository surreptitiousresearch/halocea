#ifndef SINGLE_PLAYER_PROGRESS_FLAGS_H
#define SINGLE_PLAYER_PROGRESS_FLAGS_H

/* single_player_progress_flags — single_player_progress flags.
 * DB-verified: types_enum_values _399166601856AF194D21EBF3B993B504 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum single_player_progress_flags {
	_single_player_map_a10 = 0,
	_single_player_map_a30 = 1,
	_single_player_map_a50 = 2,
	_single_player_map_b30 = 3,
	_single_player_map_b40 = 4,
	_single_player_map_c10 = 5,
	_single_player_map_c20 = 6,
	_single_player_map_c40 = 7,
	_single_player_map_d20 = 8,
	_single_player_map_d40 = 9,
	NUMBER_OF_SINGLE_PLAYER_LEVELS = 10,
	_single_player_normal_completed_bit = 0,
	_single_player_hard_completed_bit = 1,
	_single_player_impossible_completed_bit = 2,
	NUMBER_OF_SINGLE_PLAYER_FLAGS = 3,
};

#endif /* SINGLE_PLAYER_PROGRESS_FLAGS_H */

// single_player_progress_flags: DB-verified via types_enum_values _399166601856AF194D21EBF3B993B504
