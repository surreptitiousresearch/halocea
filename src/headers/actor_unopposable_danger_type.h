#ifndef ACTOR_UNOPPOSABLE_DANGER_TYPE_H
#define ACTOR_UNOPPOSABLE_DANGER_TYPE_H

/* actor_unopposable_danger_type — escalation ladder scoring how dangerous a "no longer opposable"
 * enemy is to an actor (actor_unopposable_enemy.unopposable_type and the per-prop danger score).
 * DB-verified: types_enum_values _4AC531F8CE22A274E3A00D8E7532E756 (anonymous compiled enum, names
 * sans leading _). Values 0..4 come from the prop's own combat state; 5..9 are hysteresis/escalation
 * tiers raised in actor_emotion_unopposable_retreat. */
enum actor_unopposable_danger_type {
	_actor_unopposable_danger_none = 0,
	_actor_unopposable_danger_visible = 1,
	_actor_unopposable_danger_shooting = 2,
	_actor_unopposable_danger_shooting_nearby = 3,
	_actor_unopposable_danger_damaging = 4,
	_actor_unopposable_danger_retreat = 5,
	_actor_unopposable_danger_friends_retreating = 6,
	_actor_unopposable_danger_time_triggered = 7,
	_actor_unopposable_danger_immediately_triggered = 8,
	_actor_unopposable_danger_friends_killed = 9,
	NUMBER_OF_ACTOR_UNOPPOSABLE_DANGER_TYPES = 10,
};

#endif /* ACTOR_UNOPPOSABLE_DANGER_TYPE_H */
