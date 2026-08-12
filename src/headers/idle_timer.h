#pragma once

/* idle_timer — the timer_type selector of actor_look_idle_timer() @0x837FB248. That function
 * picks its time range straight off this value: 0 -> actor_idle_looking.idle_facing_time_*,
 * 1 -> idle_aim_time_*, 2 -> idle_look_time_* (>= 3 is the degenerate out-of-domain branch),
 * which is the domain below member for member. Its three callers store the result into
 * actor->control.idle_facing_timer / idle_major_timer / idle_minor_timer.
 * Source: types_enum_values $AEA3B3BD2824766B3A00E3302B1343B3 (DB-verified). */
enum idle_timer {
	_idle_timer_facing = 0,
	_idle_timer_aiming = 1,
	_idle_timer_looking = 2,
};

// idle_timer: DB-verified via types_enum_values $AEA3B3BD2824766B3A00E3302B1343B3
