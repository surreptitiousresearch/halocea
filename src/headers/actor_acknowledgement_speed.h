#pragma once
/* actor_acknowledgement_speed — how fast an actor accrues awareness of a prop, the value space of
 * global_acknowledgement_speeds[knowledge][perception]. actor_perception_update selects the
 * awareness delta from this: never -> 0.0, non_combat/guard/combat -> the actor definition's
 * runtime_awareness_delta_non_combat / _guard / _combat, instant -> 1.0 (acknowledge this tick).
 * Source: DB types_enum_values _BF0080FDFC6892546396A0173488D3B1 (anonymous compiled enum). */

enum actor_acknowledgement_speed {
	_actor_acknowledgement_never = 0,
	_actor_acknowledgement_non_combat = 1,
	_actor_acknowledgement_guard = 2,
	_actor_acknowledgement_combat = 3,
	_actor_acknowledgement_instant = 4,
	NUMBER_OF_ACTOR_ACKNOWLEDGEMENT_SPEEDS = 5,
};

// actor_acknowledgement_speed: DB-verified via types_enum_values _BF0080FDFC6892546396A0173488D3B1
