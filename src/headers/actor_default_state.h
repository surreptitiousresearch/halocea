#ifndef ACTOR_DEFAULT_STATE_H
#define ACTOR_DEFAULT_STATE_H

enum actor_default_state {
	actor_default_state_none = 0,
	actor_default_state_asleep = 1,
	actor_default_state_alert = 2,
	actor_default_state_moving_repeat_position = 3,
	actor_default_state_moving_loop = 4,
	actor_default_state_moving_loop_back_and_forth = 5,
	actor_default_state_moving_loop_randomly = 6,
	actor_default_state_moving_randomly = 7,
	actor_default_state_guarding = 8,
	actor_default_state_guarding_at_guard_point = 9,
	actor_default_state_searching = 10,
	actor_default_state_fleeing = 11,
	number_of_actor_default_states = 12,
};

#endif /* ACTOR_DEFAULT_STATE_H */

// actor_default_state: DB-verified via types_enum_values _F7EF6512BC3C34595D6808ECB999616D (anonymous compiled enum, names sans leading _)
