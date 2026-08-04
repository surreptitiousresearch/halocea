#ifndef GAME_DIFFICULTY_H
#define GAME_DIFFICULTY_H

/* game_difficulty — the campaign difficulty level.
 * Source: reference enum _F669C9C7B8AC2FCE4CE1CEBF490261B0 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum game_difficulty {
	game_difficulty_level_easy = 0,
	game_difficulty_level_normal = 1,
	game_difficulty_level_hard = 2,
	game_difficulty_level_impossible = 3,
	number_of_game_difficulty_levels = 4,
};

#endif /* GAME_DIFFICULTY_H */

// game_difficulty: DB-verified via types_enum_values _F669C9C7B8AC2FCE4CE1CEBF490261B0 (anonymous compiled enum, names sans leading _)
