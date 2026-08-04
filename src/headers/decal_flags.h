#ifndef DECAL_FLAGS_H
#define DECAL_FLAGS_H

/* decal_flags - bit indices for the decal.flags word.
 * DB-verified: types_enum_values _C743D5EF384CCD19F6F6A1A12E9381B1 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum decal_flags {
	_decal_locked_bit = 0,
	_decal_permanent_bit = 1,
	NUMBER_OF_DECAL_FLAGS = 2,
};

#endif /* DECAL_FLAGS_H */

// decal_flags: DB-verified via types_enum_values _C743D5EF384CCD19F6F6A1A12E9381B1
