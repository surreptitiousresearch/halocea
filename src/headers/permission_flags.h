#ifndef PERMISSION_FLAGS_H
#define PERMISSION_FLAGS_H

/* permission_flags - bit indices for the permission.flags word.
 * DB-verified: types_enum_values _73130D07748C3137A3522564E3E9B5E7 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum permission_flags {
	_permission_read_bit = 0,
	_permission_write_bit = 1,
	_permission_append_bit = 2,
	NUMBER_OF_PERMISSION_FLAGS = 3,
};

#endif /* PERMISSION_FLAGS_H */

// permission_flags: DB-verified via types_enum_values _73130D07748C3137A3522564E3E9B5E7
