#pragma once
/* actor_evade — evasion/dive direction selector (actor_action_try_to_dive.c, actor_move_try_evasion_direction).
 * Source: headers_ref _D37307CA04FA5397D09570989E33774D. */

enum actor_evade {
	_actor_evade_left = 0,
	_actor_evade_right = 1,
	_actor_evade_forward = 2,
	_actor_evade_back = 3,
	_actor_evade_random_side = 4,
};

// actor_evade: DB-verified via types_enum_values _D37307CA04FA5397D09570989E33774D (anonymous compiled enum, names sans leading _)
