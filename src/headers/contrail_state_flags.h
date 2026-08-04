#ifndef CONTRAIL_STATE_FLAGS_H
#define CONTRAIL_STATE_FLAGS_H

/* contrail_state_flags - bit indices for the contrail_state.flags word.
 * DB-verified: types_enum_values _E9D3038F2E2E09C47776F15FB5AE9A8B carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum contrail_state_flags {
	_contrail_state_duration_bit = 0,
	_contrail_state_duration_delta_bit = 1,
	_contrail_state_transition_duration_bit = 2,
	_contrail_state_transition_duration_delta_bit = 3,
	_contrail_state_width_bit = 4,
	_contrail_state_color_bit = 5,
	NUMBER_OF_CONTRAIL_POINT_STATE_SCALEABLE_VALUES = 6,
};

#endif /* CONTRAIL_STATE_FLAGS_H */

// contrail_state_flags: DB-verified via types_enum_values _E9D3038F2E2E09C47776F15FB5AE9A8B
