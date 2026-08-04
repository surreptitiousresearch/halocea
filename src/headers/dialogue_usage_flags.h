#ifndef DIALOGUE_USAGE_FLAGS_H
#define DIALOGUE_USAGE_FLAGS_H

/* dialogue_usage_flags — dialogue_usage flags.
 * DB-verified: types_enum_values _5AB6EA521C637FB95D3B2DD7EF4A4233 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum dialogue_usage_flags {
	_dialogue_usage_lookup_bit = 0,
	_dialogue_usage_force_bit = 1,
	_dialogue_usage_immediate_notify_bit = 2,
	_dialogue_usage_player_bit = 3,
	_dialogue_usage_same_vehicle_bit = 4,
	_dialogue_usage_allow_subject_bit = 5,
	_dialogue_usage_override_scripted_bit = 6,
	NUMBER_OF_DIALOGUE_USAGE_FLAGS = 7,
};

#endif /* DIALOGUE_USAGE_FLAGS_H */

// dialogue_usage_flags: DB-verified via types_enum_values _5AB6EA521C637FB95D3B2DD7EF4A4233
