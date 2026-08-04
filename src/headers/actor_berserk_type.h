#ifndef ACTOR_BERSERK_TYPE_H
#define ACTOR_BERSERK_TYPE_H

enum actor_berserk_type {
	actor_berserk_none = 0,
	actor_berserk_attacking = 1,
	actor_berserk_proximity = 2,
	actor_berserk_damage = 3,
	actor_berserk_attached_projectiles = 4,
	actor_berserk_never = 5,
	number_of_actor_berserk_types = 6,
};

#endif /* ACTOR_BERSERK_TYPE_H */

// actor_berserk_type: DB-verified via types_enum_values _E72134F44A11D41BD40CB9CEE712583B (anonymous compiled enum, names sans leading _)
