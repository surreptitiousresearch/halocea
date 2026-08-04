#ifndef SAVED_GAME_FILES_PATH_TYPE_H
#define SAVED_GAME_FILES_PATH_TYPE_H

/* saved_game_files_path_type — selector for saved_game_files_get_path().
 * DB-verified: types_enum_values _08FB2D773F21D0757E756730442171FC (value_name spellings verbatim). */
enum saved_game_files_path_type {
	_saved_game_files_path_type_memory_unit_root = 0,
	_saved_game_files_path_type_savegame_root = 1,
	_saved_game_files_path_type_saved_files_directory = 2,
	_saved_game_files_path_type_player_profiles_directory = 3,
	_saved_game_files_path_type_default_player_profile_path = 4,
	_saved_game_files_path_type_playlist_directory = 5,
	_saved_game_files_path_type_default_playlist_path = 6,
	_saved_game_files_path_type_player1_profile_last_used_file = 7,
	_saved_game_files_path_type_mp_variant_last_used_file = 8,
	_saved_game_files_path_type_mp_map_last_used_file = 9,
	NUMBER_OF_SAVED_GAME_FILES_PATH_TYPES = 10,
};

#endif /* SAVED_GAME_FILES_PATH_TYPE_H */
