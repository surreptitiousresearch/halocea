#pragma once

/* detonation_timer — value of _projectile_definition.detonation_timer_mode (int16_t @0x04, the tag's
 * "detonation timer starts" field): when the projectile's detonation_timer begins to advance.
 * projectile_update (@0x8375BEF0) ages the timer unconditionally for _detonation_timer_normal, and
 * for the other two modes only once the projectile's _projectile_stopped_after_collision_bit is set
 * (it has bounced / come to rest). Source: types_enum_values $80A739E559F2509AC5DC5CB4DBD6475F
 * (DB-verified). */
enum detonation_timer {
	_detonation_timer_normal = 0,
	_detonation_timer_once_bounced = 1,
	_detonation_timer_when_at_rest = 2,
	NUMBER_OF_DETONATION_TIMER_MODES = 3,
};

// detonation_timer: DB-verified via types_enum_values $80A739E559F2509AC5DC5CB4DBD6475F
