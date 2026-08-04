#ifndef GAME_ENGINE_PLACE_CONSTANTS_H
#define GAME_ENGINE_PLACE_CONSTANTS_H

/* Game-engine place (ranking) string constants: place ordinals are clamped to
 * maximum_places; the _place_name_* values are unicode string-list indices for the
 * special standing strings. */
enum game_engine_place_constants {
	maximum_places = 32,
	_place_name_even = 64,
	_place_name_winning = 65,
	_place_name_losing = 66,
	_place_name_tied = 67,
};

#endif /* GAME_ENGINE_PLACE_CONSTANTS_H */

// game_engine_place_constants: DB-verified via types_enum_values $26EEE0E032EA047CDF55F1EDE4014C3B
