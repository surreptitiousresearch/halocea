#ifndef RECORDING_THREAD_FLAGS_H
#define RECORDING_THREAD_FLAGS_H

/* recording_thread_flags - bit indices for the recording_thread.flags word.
 * DB-verified: types_enum_values _9DC7DC0ED346EB32EE1CA78384A75866 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum recording_thread_flags {
	_recording_thread_finished_bit = 0,
	_recording_thread_killed_bit = 1,
	_recording_thread_unit_was_controlled_bit = 2,
	_recording_thread_delete_unit_on_complete_bit = 3,
	_recording_thread_hover_vehicle_on_complete_bit = 4,
	NUMBER_OF_RECORDING_THREAD_FLAGS = 5,
};

#endif /* RECORDING_THREAD_FLAGS_H */

// recording_thread_flags: DB-verified via types_enum_values _9DC7DC0ED346EB32EE1CA78384A75866
