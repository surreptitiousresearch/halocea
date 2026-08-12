#pragma once

/* game_win_type — the tri-state result of the game engine's per-player win query
 * (game_engine_did_player_win / game_engine_did_player_win_default), switched over by
 * game_engine_generate_title_string @0x83750800 to pick the postgame title string.
 * Source: types_enum_values game_win_type (DB-verified, members verbatim in DB order). */
enum game_win_type {
	_player_won = 1,
	_player_lost = 0,
	_player_tied = -1,
};

// game_win_type: DB-verified via types_enum_values game_win_type
