#ifndef GAME_ENGINE_FLAGS_H
#define GAME_ENGINE_FLAGS_H

/* game_engine_flags — bit indices for the game_engine flags word.
 * DB-verified: types_enum_values _1047E9A4C5DC367E4B8C0934DA946A92 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum game_engine_flags {
	_game_engine_disable_dynamic_light_bit = 0,
	_game_engine_disable_integrated_lights_bit = 1,
	_game_engine_5_or_more_players_bit = 2,
	_game_engine_9_or_more_players_bit = 3,
	_game_engine_game_over_sound_disabled_bit = 4,
};

#endif /* GAME_ENGINE_FLAGS_H */

// game_engine_flags: DB-verified via types_enum_values _1047E9A4C5DC367E4B8C0934DA946A92
