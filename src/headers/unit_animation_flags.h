#ifndef UNIT_ANIMATION_FLAGS_H
#define UNIT_ANIMATION_FLAGS_H

/* unit_animation_flags — bit indices for _unit_datum.unit.animation.flags runtime (unit.animation.flags).
 * Source: reference enum _BCBAF51627CE75D5B0CF9C3397D033BA (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum unit_animation_flags {
	_unit_animation_postpone_weapon_ik_until_interpolation_ends_bit = 0x0,
	_unit_animation_showing_acceleration_bit = 0x1,
	_unit_animation_ignore_translation_bit = 0x2,
	_unit_animation_fallen_on_front_bit = 0x3,
	NUMBER_OF_UNIT_ANIMATION_FLAGS = 0x4,
};

#endif /* UNIT_ANIMATION_FLAGS_H */

// unit_animation_flags: DB-verified via types_enum_values _BCBAF51627CE75D5B0CF9C3397D033BA (anonymous compiled enum, names sans leading _)
