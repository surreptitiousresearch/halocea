#ifndef ACTOR_ACTION_H
#define ACTOR_ACTION_H

enum actor_action {
	actor_action_none = 0,
	actor_action_sleep = 1,
	actor_action_alert = 2,
	actor_action_fight = 3,
	actor_action_flee = 4,
	actor_action_uncover = 5,
	actor_action_guard = 6,
	actor_action_search = 7,
	actor_action_wait = 8,
	actor_action_vehicle = 9,
	actor_action_charge = 10,
	actor_action_obey = 11,
	actor_action_converse = 12,
	actor_action_avoid = 13,
	number_of_actor_actions = 14,
};

#endif /* ACTOR_ACTION_H */

// actor_action: DB-verified via types_enum_values _F8A4FB32A21F4CE1C69FC98D676078C5 (anonymous compiled enum, names sans leading _)
