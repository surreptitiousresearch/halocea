#pragma once

/* actor_facing — enum drain Wave E (2026-08-13): named for the fix sites this wave applies;
 * see the consumer TUs including this header for the per-site evidence.
 * Source: types_enum_values $53D606AE39E79059735B54401B32FAA6 (DB-verified, members verbatim in DB order). */
enum actor_facing {
	_actor_facing_forward = 0,
	_actor_facing_backward = 1,
	_actor_facing_left = 2,
	_actor_facing_right = 3,
	_actor_facing_free = 4,
	NUMBER_OF_ACTOR_FACINGS = 5,
};

// actor_facing: DB-verified via types_enum_values $53D606AE39E79059735B54401B32FAA6
