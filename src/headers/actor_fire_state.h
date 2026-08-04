#ifndef ACTOR_FIRE_STATE_H
#define ACTOR_FIRE_STATE_H

enum actor_fire_state {
	actor_fire_state_none = 0,
	actor_fire_state_holding = 1,
	actor_fire_state_bursting = 2,
	actor_fire_state_pausing = 3,
	actor_fire_state_wild = 4,
	number_of_actor_fire_states = 5,
};

#endif /* ACTOR_FIRE_STATE_H */

// actor_fire_state: DB-verified via types_enum_values _CFFD11D4D29F96498D7007F859EA014C (anonymous compiled enum, names sans leading _)
