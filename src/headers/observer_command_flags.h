#ifndef OBSERVER_COMMAND_FLAGS_H
#define OBSERVER_COMMAND_FLAGS_H

/* observer_command_flags - bit indices for the observer_command.flags word.
 * DB-verified: types_enum_values _9D27FE54618CE19563F64B1F749F86DB carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum observer_command_flags {
	_observer_command_valid_bit = 0,
	_observer_command_force_under_media_bit = 1,
	_observer_command_force_above_media_bit = 2,
	_observer_command_force_time_bit = 3,
	_observer_command_ignore_obstructions_bit = 4,
	_observer_command_freeze_camera_bit = 5,
	NUMBER_OF_OBSERVER_COMMAND_FLAGS = 6,
};

#endif /* OBSERVER_COMMAND_FLAGS_H */

// observer_command_flags: DB-verified via types_enum_values _9D27FE54618CE19563F64B1F749F86DB
