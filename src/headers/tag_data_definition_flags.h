#ifndef TAG_DATA_DEFINITION_FLAGS_H
#define TAG_DATA_DEFINITION_FLAGS_H

/* tag_data_definition_flags — bit indices for the tag_data_definition flags word.
 * DB-verified: types_enum_values _10B697942FA1060C4CF69D9C39D4D412 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum tag_data_definition_flags {
	_tag_data_cached_bit = 0,
	_tag_data_string_bit = 1,
	_tag_data_not_added_to_monolithic_file_bit = 2,
	NUMBER_OF_TAG_DATA_DEFINITION_FLAGS = 3,
};

#endif /* TAG_DATA_DEFINITION_FLAGS_H */

// tag_data_definition_flags: DB-verified via types_enum_values _10B697942FA1060C4CF69D9C39D4D412
