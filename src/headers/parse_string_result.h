#ifndef PARSE_STRING_RESULT_H
#define PARSE_STRING_RESULT_H

/* parse_string_result — token classification returned by parse_unicode_string (state->result).
 * DB-verified via types_enum_values $403D44D8579E3FC5128F842C44A4E0BF (DB counter name
 * NUMBER_OF_PARSE_STRING_STATES; enum tag renamed here because the parse_string_state tag is the
 * cursor struct). NOTE: the ASCII parse_string @0x8376A0C8 does NOT follow these assignments
 * (its 2=printable/6=word-break are inverted vs this enum) — see the note in parse_string.c;
 * only the unicode path binds these names. */
enum parse_string_result {
	_parsed_end_of_string = 0,
	_parsed_end_of_line = 1,
	_parsed_end_of_word = 2,
	_parsed_end_of_column = 3,
	_parsed_justification_change = 4,
	_parsed_color_change = 5,
	_parsed_character = 6,
	_parsed_style_change = 7,
	NUMBER_OF_PARSE_STRING_STATES = 8,
};

#endif /* PARSE_STRING_RESULT_H */
