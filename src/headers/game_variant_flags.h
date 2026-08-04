#ifndef GAME_VARIANT_FLAGS_H
#define GAME_VARIANT_FLAGS_H

/* game_variant_flags — bit indices for game_variant.flags.
 * DB-verified: types_enum_values _D9EED5400567BC7BB63820DAFB4425F0 (value_name spellings verbatim). */
enum game_variant_flags {
	_variant_is_system_default_bit = 0,
	NUMBER_OF_GAME_VARIANT_FLAGS = 1,
	_default_variant_index_mask = 0xFF00,
};

#endif /* GAME_VARIANT_FLAGS_H */
