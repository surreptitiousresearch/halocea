#ifndef BUILD_SPRITES_INTERNAL_FLAGS_H
#define BUILD_SPRITES_INTERNAL_FLAGS_H

/* build_sprites_internal_flags — internal state bits stored in build_sprite_data.flags alongside the
 * caller-supplied build_sprite_flags. build_sprites_begin sets the valid bit (flags |= 1<<2); the matching
 * build_sprites_end clears it. Distinct enum from build_sprite_flags/build_sprites_flags — same field, but
 * these are the builder's own runtime markers.
 * DB-verified: types_enum_values _A2AAABCE201F9E33F8659420A9FDD3CE carries _build_sprites_valid_bit=2 verbatim.
 * Values are BIT INDICES — use as (1u << _bit). */
enum build_sprites_internal_flags {
	_build_sprites_valid_bit = 2,
};

#endif /* BUILD_SPRITES_INTERNAL_FLAGS_H */

// build_sprites_internal_flags: DB-verified via types_enum_values _A2AAABCE201F9E33F8659420A9FDD3CE
