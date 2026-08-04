#ifndef ACTOR_DEFINITION_FLAGS2_FLAGS_H
#define ACTOR_DEFINITION_FLAGS2_FLAGS_H

/* actor_definition_flags2_flags - bit indices for the actor_definition_flags2.flags word.
 * DB-verified: types_enum_values _8CF8C4A8BEFC62CE4E5CDF76867E9C1E carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum actor_definition_flags2_flags {
	_actor_definition_flags2_avoid_all_enemy_attack_vectors_bit = 0,
	_actor_definition_flags2_must_stand_to_fire_bit = 1,
	_actor_definition_flags2_must_stop_to_fire_bit = 2,
	_actor_definition_flags2_disallow_vehicle_combat_bit = 3,
	_actor_definition_flags2_pathfinding_ignores_danger_bit = 4,
	_actor_definition_flags2_panic_in_groups_bit = 5,
	_actor_definition_flags2_no_corpse_shooting_bit = 6,
	NUMBER_OF_ACTOR_DEFINITION_FLAGS2 = 7,
};

#endif /* ACTOR_DEFINITION_FLAGS2_FLAGS_H */

// actor_definition_flags2_flags: DB-verified via types_enum_values _8CF8C4A8BEFC62CE4E5CDF76867E9C1E
