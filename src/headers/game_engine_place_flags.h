#ifndef GAME_ENGINE_PLACE_FLAGS_H
#define GAME_ENGINE_PLACE_FLAGS_H

/* game_engine_place_flags — bit indices for game_engine_place.flags (set by game_engine_get_place,
 * consumed by get_place_name). Source: DB enum game_engine_place_flags (types_enum_values, verbatim). */
enum game_engine_place_flags {
	_place_tied = 0,        /* tied with at least one other participant at this score */
	_place_all_tied = 1,    /* every participant has the same score */
	_place_two_groups = 2,  /* exactly two scoring participants (head-to-head) */
	_place_team = 3,        /* team-mode standing (participants are teams) */
};

#endif /* GAME_ENGINE_PLACE_FLAGS_H */
