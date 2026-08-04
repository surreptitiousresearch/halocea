#ifndef ACTOR_GUARD_LOCATION_TYPE_H
#define ACTOR_GUARD_LOCATION_TYPE_H

/* actor_guard_location_type - value space of guard_state_data.guard_location_type.
 * DB-verified via types_enum_values _0B8DC87702F3BD58FFB241CD74A205BA
 * (anonymous compiled enum). */
enum actor_guard_location_type {
	_actor_guard_location_none = 0,
	_actor_guard_location_current = 1,
	_actor_guard_location_point = 2,
	_actor_guard_location_firing_position = 3,
	NUMBER_OF_ACTOR_GUARD_LOCATION_TYPES = 4,
};

#endif /* ACTOR_GUARD_LOCATION_TYPE_H */
