#ifndef GAME_ENGINE_VARIANT_FLAGS_H
#define GAME_ENGINE_VARIANT_FLAGS_H

/* game_engine_variant_flags — bit indices for the multiplayer game-engine variant flags word.
 * DB-verified: types_enum_values _8A567A418665F44AD14EE33258F0DC07 carries these named bit
 * indices verbatim (DB sentinel: NUMBER_OF_GAME_ENGINE_FLAGS). Renamed from the bare
 * game_engine_flags to avoid clashing with the pre-existing unrelated game_engine_flags enum
 * (_1047E9A4C5DC367E4B8C0934DA946A92, runtime display flags).
 * Values are BIT INDICES — use as (1u << _bit). */
enum game_engine_variant_flags {
	_game_engine_spawn_point_obeys_teams_bit = 0,
	_game_engine_show_kills_with_score_bit = 1,
	NUMBER_OF_GAME_ENGINE_FLAGS = 2,
};

#endif /* GAME_ENGINE_VARIANT_FLAGS_H */

// game_engine_variant_flags: DB-verified via types_enum_values _8A567A418665F44AD14EE33258F0DC07
