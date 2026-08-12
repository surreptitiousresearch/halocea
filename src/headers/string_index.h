#pragma once

/* string_index — index into the localization string_list tag (_interface_string_list_localization),
 * the tag draw_string_initialize_for_new_map loads into font_drawing_globals.string_list_index. Its
 * first seven entries are the localization header: that function reads string 0 and atoi()s it into
 * the language code, and draw_string_get_string returns string_list_get_string(..., index + 7) — the
 * bitmap-text strings begin right after those seven header entries, which is exactly what
 * _string_index_first_bitmap_text_string = 7 says.
 * Source: types_enum_values $0A1EC5472867DD009E1762992ADFF169 (DB-verified). */
enum string_index {
	_string_index_language = 0,
	_string_index_country_code = 1,
	_string_index_truncation_string = 2,
	_string_index_illegal_truncation_characters = 3,
	_string_index_can_end_words = 4,
	_string_index_cannot_end_words = 5,
	_string_index_cannot_begin_words = 6,
	_string_index_first_bitmap_text_string = 7,
};

// string_index: DB-verified via types_enum_values $0A1EC5472867DD009E1762992ADFF169
