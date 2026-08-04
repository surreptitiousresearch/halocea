#ifndef SAVED_GAME_FILE_TYPE_H
#define SAVED_GAME_FILE_TYPE_H

/* saved_game_file_type — selects which category of saved-game file the
 * saved_game_files layer enumerates/creates.
 * DB-verified: types_enum_values _BD43A4B4B87F771BC8B7B032BD468D87 (value_name spellings verbatim). */
enum saved_game_file_type {
	_saved_game_file_type_player_profile = 0,
	_saved_game_file_type_play_list = 1,
	NUMBER_OF_SAVED_GAME_FILE_TYPES = 2,
};

#endif /* SAVED_GAME_FILE_TYPE_H */
