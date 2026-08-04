#ifndef ACTOR_COMBAT_TRANSITION_TYPE_H
#define ACTOR_COMBAT_TRANSITION_TYPE_H

/* actor_combat_transition_type — priority of the stimulus requesting an actor transition into combat,
 * stored in actor_stimulus_data.combat_transition. Higher wins; a request only latches when it outranks
 * the pending one. DB-verified via types_enum_values _9D13EF93874683969F70339D483D59C1. */
enum actor_combat_transition_type {
	_actor_stimulus_combat_none = 0,
	_actor_stimulus_combat_friend = 1,
	_actor_stimulus_combat_body = 2,
	_actor_stimulus_combat_impact = 3,
	_actor_stimulus_combat_danger = 4,
	_actor_stimulus_combat_damage = 5,
	_actor_stimulus_combat_enemy = 6,
	NUMBER_OF_ACTOR_COMBAT_TRANSITION_TYPES = 7,
};

#endif /* ACTOR_COMBAT_TRANSITION_TYPE_H */
