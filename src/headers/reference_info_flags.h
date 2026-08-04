#ifndef REFERENCE_INFO_FLAGS_H
#define REFERENCE_INFO_FLAGS_H

/* reference_info_flags — reference_info flags.
 * DB-verified: types_enum_values _19594DD35D63DC44799B0D605D50E24D carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum reference_info_flags {
	_has_filename_bit = 0,
	NUMBER_OF_REFERENCE_INFO_FLAGS = 1,
};

#endif /* REFERENCE_INFO_FLAGS_H */

// reference_info_flags: DB-verified via types_enum_values _19594DD35D63DC44799B0D605D50E24D
