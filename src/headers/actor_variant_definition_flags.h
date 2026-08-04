#ifndef ACTOR_VARIANT_DEFINITION_FLAGS_H
#define ACTOR_VARIANT_DEFINITION_FLAGS_H

/* actor_variant_definition_flags — bit indices for actor_variant_definition.flags.
 * Source: reference enum _C0D046F5AF438DC87047EF10233482D4 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum actor_variant_definition_flags {
	_actor_variant_definition_can_shoot_while_flying_bit = 0x0,
	_actor_variant_definition_color_interpolate_hsv_bit = 0x1,
	_actor_variant_definition_has_unlimited_grenades_bit = 0x2,
	_actor_variant_definition_movement_switching_try_to_stay_with_friends_bit = 0x3,
	_actor_variant_definition_active_camouflage_bit = 0x4,
	_actor_variant_definition_super_active_camouflage_bit = 0x5,
	_actor_variant_definition_cannot_use_ranged_weapons_bit = 0x6,
	_actor_variant_definition_prefer_passenger_seat_bit = 0x7,
	NUMBER_OF_ACTOR_VARIANT_DEFINITION_FLAGS = 0x8,
};

#endif /* ACTOR_VARIANT_DEFINITION_FLAGS_H */

// actor_variant_definition_flags: DB-verified via types_enum_values _C0D046F5AF438DC87047EF10233482D4 (anonymous compiled enum, names sans leading _)
