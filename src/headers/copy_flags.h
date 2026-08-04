#ifndef COPY_FLAGS_H
#define COPY_FLAGS_H

/* copy_flags — copy flags.
 * DB-verified: types_enum_values _3C926968815E8FF14D5198E407035E99 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum copy_flags {
	_copy_write_failed_bit = 0,
	_copy_read_failed_bit = 1,
	_copy_decompression_failed_bit = 2,
	NUMBER_OF_COPY_FLAGS = 3,
	_copy_error_first_bit = 0,
	_copy_error_last_bit = 2,
};

#endif /* COPY_FLAGS_H */

// copy_flags: DB-verified via types_enum_values _3C926968815E8FF14D5198E407035E99
