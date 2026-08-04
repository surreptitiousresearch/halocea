#ifndef BITMAP_GROUP_FLAGS_H
#define BITMAP_GROUP_FLAGS_H

/* bitmap_group_flags - bit indices for the bitmap_group.flags word.
 * DB-verified: types_enum_values _D884302C23AF74FEAD61EB74A8417A9F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum bitmap_group_flags {
	_bitmap_group_diffusion_dither_bit = 0,
	_bitmap_group_disable_vector_compression_bit = 1,
	_bitmap_group_uniform_sprite_sequences_bit = 2,
	_bitmap_group_extract_sprites_filthy_bug_fix_bit = 3,
	NUMBER_OF_BITMAP_GROUP_FLAGS = 4,
};

#endif /* BITMAP_GROUP_FLAGS_H */

// bitmap_group_flags: DB-verified via types_enum_values _D884302C23AF74FEAD61EB74A8417A9F
