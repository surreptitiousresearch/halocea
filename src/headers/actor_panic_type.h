#ifndef ACTOR_PANIC_TYPE_H
#define ACTOR_PANIC_TYPE_H

/* actor_panic_type - value space of stimuli/flee panic_type members. */
enum actor_panic_type {
	_actor_panic_none = 0,
	_actor_panic_damage = 1,
	_actor_panic_friend_fleeing = 2,
	_actor_panic_friend_same_type_killed = 3,
	_actor_panic_no_cover_available = 4,
	_actor_panic_unopposable_enemy = 5,
	_actor_panic_platoon_retreating = 6,
	_actor_panic_surprise = 7,
	_actor_panic_friend_leader_type_killed = 8,
	_actor_panic_grenade_attached_to_us = 9,
	_actor_panic_delayed_projectile_attached_to_us = 10,
	_actor_panic_melee_attached_to_us = 11,
	_actor_panic_burning_to_death = 12,
	_actor_panic_scripted = 13,
	_actor_panic_never = 14,
	NUMBER_OF_ACTOR_PANIC_TYPES = 15,
};

#endif /* ACTOR_PANIC_TYPE_H */

// actor_panic_type: DB-verified via types_enum_values _769A795F86FAE4B83CA6DDAA0ABB47BF (anonymous compiled enum, names sans leading _)
