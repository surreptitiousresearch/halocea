#ifndef ACTOR_DEFINITION_FLAGS_H
#define ACTOR_DEFINITION_FLAGS_H

/* actor_definition_flags — bit indices for actor_definition.flags (unsigned int @0x00).
 * DB-verified: types_enum_values _555FCBCDEED57090E75721F1769C088C carries these 32 named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum actor_definition_flags {
	_actor_definition_can_see_in_darkness_bit = 0,
	_actor_definition_sneak_uncovering_target_bit = 1,
	_actor_definition_sneak_uncovering_pursuit_position_bit = 2,
	_actor_definition_unused_bit = 3,
	_actor_definition_shoot_at_targets_last_location_bit = 4,
	_actor_definition_crouch_try_not_to_move_bit = 5,
	_actor_definition_crouch_when_noncombat_bit = 6,
	_actor_definition_crouch_when_guarding_bit = 7,
	_actor_definition_unused2_bit = 8,
	_actor_definition_must_crouch_to_fire_bit = 9,
	_actor_definition_panic_on_surprise_bit = 10,
	_actor_definition_always_charge_bit = 11,
	_actor_definition_helps_players_in_vehicles_bit = 12,
	_actor_definition_start_firing_before_aligned_bit = 13,
	_actor_definition_standing_must_move_forward_bit = 14,
	_actor_definition_crouching_must_move_forward_bit = 15,
	_actor_definition_defensive_crouch_while_charging_bit = 16,
	_actor_definition_stalking_behavior_bit = 17,
	_actor_definition_stalking_freeze_when_exposed_bit = 18,
	_actor_definition_always_berserk_in_attacking_mode_bit = 19,
	_actor_definition_berserk_use_panic_movement_bit = 20,
	_actor_definition_flying_bit = 21,
	_actor_definition_panicked_by_unopposable_enemy_bit = 22,
	_actor_definition_crouch_when_hiding_from_unopposable_enemy_bit = 23,
	_actor_definition_charge_in_attacking_mode_bit = 24,
	_actor_definition_dive_off_ledges_bit = 25,
	_actor_definition_swarm_actor_bit = 26,
	_actor_definition_suicidal_melee_attack_bit = 27,
	_actor_definition_cannot_move_crouching_bit = 28,
	_actor_definition_fixed_crouch_facing_bit = 29,
	_actor_definition_crouch_in_line_of_fire_bit = 30,
	_actor_definition_avoid_friend_line_of_fire_bit = 31,
	NUMBER_OF_ACTOR_DEFINITION_FLAGS = 32,
};

#endif /* ACTOR_DEFINITION_FLAGS_H */

// actor_definition_flags: DB-verified via types_enum_values _555FCBCDEED57090E75721F1769C088C (anonymous compiled enum, names sans leading _)
