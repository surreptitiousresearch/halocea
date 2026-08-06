#ifndef BUILD_SPRITE_FLAGS_H
#define BUILD_SPRITE_FLAGS_H

/* build_sprite_flags — the PER-SPRITE mirror word passed as the `flags` ARGUMENT to
 * build_sprite() / build_sprite_rotational(). Source: headers_ref _D9CCB6DD799C90775829C93F05D29183.
 * Values are BIT INDICES — use as (1u << _bit).
 *
 * SCOPE CORRECTION (2026-08-06): this header previously claimed to be the enum for
 * `build_sprite_data.flags` "across all six build_sprite_* files", on the strength of bit 0
 * matching a screen/viewer-space branch everywhere. That was a GUESS and it was wrong. The
 * *state* word in `build_sprite_data.flags` is the caller's argument to `build_sprites_begin`
 * (`data->flags = flags | (1u << _build_sprites_valid_bit)`), i.e. `build_sprites_flags`
 * (DB-verified _1DEC01DC…: screen_space=0, first_person=1), OR-ed with
 * `build_sprites_internal_flags` (DB-verified _A2AAABCE…: valid=2).
 *
 * The decisive argument is structural: `_build_sprites_valid_bit == 2` would COLLIDE with
 * `_build_sprite_v_mirror_bit == 2` below, whereas `build_sprites_flags` (bits 0,1) dovetails
 * with it exactly. Corroborated by render_particles.c passing first-person as bit 1 into
 * build_sprites_begin, which build_sprites_end then relocates to the rasterizer's own
 * first-person bit. Bit 0 carries the same value under either enum, so the six mis-typed sites
 * were behaviour-neutral — a naming/fidelity defect, not a bug.
 *
 * Use this enum ONLY for the `flags` argument of build_sprite()/build_sprite_rotational().
 * For anything read out of `build_sprite_data.flags`, use build_sprites_flags.h. */
enum build_sprite_flags {
	_build_sprite_viewer_space_bit = 0x0,
	_build_sprite_u_mirror_bit     = 0x1,
	_build_sprite_v_mirror_bit     = 0x2,
	NUMBER_OF_BUILD_SPRITE_FLAGS   = 0x3,
};

#endif /* BUILD_SPRITE_FLAGS_H */

// build_sprite_flags: DB-verified via types_enum_values _D9CCB6DD799C90775829C93F05D29183 (anonymous compiled enum, names sans leading _)
