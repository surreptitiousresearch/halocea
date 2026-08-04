#ifndef BIPED_DEFINITION_FLAGS_H
#define BIPED_DEFINITION_FLAGS_H

/* biped_definition_flags — bit indices for biped_definition.flags (via definition->biped.flags).
 * Source: reference enum _D3F47BAF4AF8D7C257679A85F84512F0 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum biped_definition_flags {
	_biped_turns_without_animating_bit = 0x0,
	_biped_uses_player_physics_bit = 0x1,
	_biped_flying_bit = 0x2,
	_biped_pill_centered_at_origin_bit = 0x3,
	_biped_spherical_bit = 0x4,
	_biped_passes_through_bipeds_bit = 0x5,
	_biped_climbs_anything_bit = 0x6,
	_biped_immune_to_falling_damage_bit = 0x7,
	_biped_rotate_while_airborne_bit = 0x8,
	_biped_uses_limp_body_physics_bit = 0x9,
	_biped_has_no_dying_airborne_bit = 0xA,
	_biped_random_speed_increase_bit = 0xB,
	_biped_uses_old_player_physics_bit = 0xC,
	NUMBER_OF_BIPED_DEFINITION_FLAGS = 0xD,
};

#endif /* BIPED_DEFINITION_FLAGS_H */

// biped_definition_flags: DB-verified via types_enum_values _D3F47BAF4AF8D7C257679A85F84512F0 (anonymous compiled enum, names sans leading _)
