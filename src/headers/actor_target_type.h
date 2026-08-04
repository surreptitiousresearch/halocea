#ifndef ACTOR_TARGET_TYPE_H
#define ACTOR_TARGET_TYPE_H

enum actor_target_type {
	actor_target_none = 0,
	actor_target_partial_enemy = 1,
	actor_target_dead_enemy = 2,
	actor_target_disregarded_orphan = 3,
	actor_target_inspected_orphan = 4,
	actor_target_uninspected_orphan = 5,
	actor_target_definite_orphan = 6,
	actor_target_acknowledged_enemy = 7,
	actor_target_clear_line_of_sight_enemy = 8,
	actor_target_potentially_dangerous_enemy = 9,
	actor_target_visible_enemy = 10,
	actor_target_damaging_enemy = 11,
	number_of_actor_target_types = 12,
};

#endif /* ACTOR_TARGET_TYPE_H */

// actor_target_type: DB-verified via types_enum_values _A9C9CF4003237F485F97CB3C5D3FF7F2 (anonymous compiled enum, names sans leading _)
