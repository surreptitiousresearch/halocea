#pragma once

/* game_matching_option — the netgame matching-option list domain: values 0..7 are the game engines (game_engine_type.h); 8..14 are the match-any options this family names. Consumer attested by disasm of match_game_type @0x8374CD68 (cmpwi 0xC/0xD/0xE arms decode _game_engine_all / _all_non_team / _all_normal exactly).
 * Source: types_enum_values $9A1AF074C10F3E4D055B24FE829A45BB (DB-verified, members verbatim in DB order). */
enum game_matching_option {
	_game_engine_ignored1 = 8,
	_game_engine_ignored2 = 9,
	_game_engine_ignored3 = 10,
	_game_engine_ignored4 = 11,
	_game_engine_all = 12,
	_game_engine_all_non_team = 13,
	_game_engine_all_normal = 14,
	NUMBER_OF_GAME_MATCHING_OPTIONS = 15,
};

// game_matching_option: DB-verified via types_enum_values $9A1AF074C10F3E4D055B24FE829A45BB
