#ifndef GAME_ENGINE_TYPE_H
#define GAME_ENGINE_TYPE_H

/* game_engine_type — the multiplayer game engine (gametype) index (game_variant.game_engine_index).
 * Source: reference enum _CC0F0496B1B8303C46F698417BFEC42C (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. Usable gametype indices are 1..5. */
enum game_engine_type {
	game_engine_none = 0,
	game_engine_ctf = 1,
	game_engine_slayer = 2,
	game_engine_oddball = 3,
	game_engine_king = 4,
	game_engine_race = 5,
	game_engine_terminator = 6,
	game_engine_stub = 7,
	number_of_game_engines = 8,
	first_usable_game_engine_index = 1,
	last_usable_game_engine_index = 5,
};

#endif /* GAME_ENGINE_TYPE_H */

// game_engine_type: DB-verified via types_enum_values _CC0F0496B1B8303C46F698417BFEC42C (anonymous compiled enum, names sans leading _)
