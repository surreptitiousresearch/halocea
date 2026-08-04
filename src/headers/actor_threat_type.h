#ifndef ACTOR_THREAT_TYPE_H
#define ACTOR_THREAT_TYPE_H

enum actor_threat_type {
	actor_threat_none = 0,
	actor_threat_visible = 1,
	actor_threat_visible_facing_me = 2,
	actor_threat_visible_aiming_at_me = 3,
	actor_threat_shooting = 4,
	actor_threat_shooting_near_me = 5,
	actor_threat_shooting_at_me = 6,
	actor_threat_extremely_close_to_me = 7,
	actor_threat_damaging_me = 8,
	number_of_actor_threat_types = 9,
};

#endif /* ACTOR_THREAT_TYPE_H */

// actor_threat_type: DB-verified via types_enum_values _E6212E4340155C95DF9356437DD30C63 (anonymous compiled enum, names sans leading _)
