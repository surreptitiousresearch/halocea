#ifndef VARIANT_IS_SYSTEM_DEFAULT_FLAGS_H
#define VARIANT_IS_SYSTEM_DEFAULT_FLAGS_H

/* variant_is_system_default_flags - bit indices for the variant_is_system_default.flags word.
 * DB-verified: types_enum_values _D9EED5400567BC7BB63820DAFB4425F0 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum variant_is_system_default_flags {
	_variant_is_system_default_bit = 0,
	NUMBER_OF_GAME_VARIANT_FLAGS = 1,
	_default_variant_index_mask = 65280,
};

#endif /* VARIANT_IS_SYSTEM_DEFAULT_FLAGS_H */

// variant_is_system_default_flags: DB-verified via types_enum_values _D9EED5400567BC7BB63820DAFB4425F0
