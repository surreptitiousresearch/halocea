#ifndef BUILD_SPRITES_FLAGS_H
#define BUILD_SPRITES_FLAGS_H

/* build_sprites_flags — build_sprites flags.
 * DB-verified: types_enum_values _1DEC01DC66AD1F419141819D45DC718E carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum build_sprites_flags {
	_build_sprites_screen_space_bit = 0,
	_build_sprites_first_person_bit = 1,
	NUMBER_OF_BUILD_SPRITES_FLAGS = 2,
};

#endif /* BUILD_SPRITES_FLAGS_H */

// build_sprites_flags: DB-verified via types_enum_values _1DEC01DC66AD1F419141819D45DC718E
