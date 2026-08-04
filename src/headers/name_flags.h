#ifndef NAME_FLAGS_H
#define NAME_FLAGS_H

/* name_flags - bit indices for the name.flags word.
 * DB-verified: types_enum_values _720F421DE1539A3955EE5164E9C466A2 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum name_flags {
	_name_directory_bit = 0,
	_name_parent_directory_bit = 1,
	_name_filename_bit = 2,
	_name_extension_bit = 3,
	NUMBER_OF_NAME_FLAGS = 4,
};

#endif /* NAME_FLAGS_H */

// name_flags: DB-verified via types_enum_values _720F421DE1539A3955EE5164E9C466A2
