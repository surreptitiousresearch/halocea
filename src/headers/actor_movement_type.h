#ifndef ACTOR_MOVEMENT_TYPE_H
#define ACTOR_MOVEMENT_TYPE_H

enum actor_movement_type {
	actor_movement_type_alert = 0,
	actor_movement_type_asleep = 1,
	actor_movement_type_combat = 2,
	actor_movement_type_panic = 3,
	actor_movement_type_flaming = 4,
	number_of_actor_movement_types = 5,
};

#endif /* ACTOR_MOVEMENT_TYPE_H */

// actor_movement_type: DB-verified via types_enum_values _ED06F08BA5D2C9FAAFBA935B38232531 (anonymous compiled enum, names sans leading _)
