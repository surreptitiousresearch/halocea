#ifndef BUILD_SPRITE_FLAGS_H
#define BUILD_SPRITE_FLAGS_H

/* build_sprite_flags — bit indices for build_sprite_data.flags (unsigned int @0x10).
 * Source: headers_ref _D9CCB6DD799C90775829C93F05D29183.
 * GUESS (inferred): this is the enum the code implements for data->flags across all six
 * build_sprite_* files. Bit 0 gates screen/viewer-space rendering in every file
 * (build_sprites_end screen-geometry vs unlit-world branch, build_sprite_get_group vertex-
 * buffer type 8 vs 6, compute_basis/compute_scale/transform_origin screen-space paths),
 * which matches _build_sprite_viewer_space_bit=0 exactly; the sibling u/v_mirror bits match
 * the per-sprite mirror flags argument threaded through build_sprite()/build_sprite_rotational().
 * Values are BIT INDICES — use as (1u << _bit).
 *
 * NOTE: build_sprites_begin sets an internal "valid/active" marker (data->flags |= 4) that
 * build_sprites_end clears (data->flags &= ~4). That bit-2 marker belongs to the separate
 * internal-flags enum _A2AAABCE201F9E33F8659420A9FDD3CE (_build_sprites_valid_bit=2), NOT the
 * v_mirror below, so that single site is intentionally left raw rather than mislabeled here. */
enum build_sprite_flags {
	_build_sprite_viewer_space_bit = 0x0,
	_build_sprite_u_mirror_bit     = 0x1,
	_build_sprite_v_mirror_bit     = 0x2,
	NUMBER_OF_BUILD_SPRITE_FLAGS   = 0x3,
};

#endif /* BUILD_SPRITE_FLAGS_H */

// build_sprite_flags: DB-verified via types_enum_values _D9CCB6DD799C90775829C93F05D29183 (anonymous compiled enum, names sans leading _)
