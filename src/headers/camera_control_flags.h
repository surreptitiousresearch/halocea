#ifndef CAMERA_CONTROL_FLAGS_H
#define CAMERA_CONTROL_FLAGS_H

/* camera_control_flags - bit indices for the camera_control.flags word.
 * DB-verified: types_enum_values _9CC0AEABA6364E542FF927C4C97AED56 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum camera_control_flags {
	_camera_control_forward_bit = 0,
	_camera_control_reverse_bit = 1,
	_camera_control_left_bit = 2,
	_camera_control_right_bit = 3,
	_camera_control_up_bit = 4,
	_camera_control_down_bit = 5,
	_camera_control_roll_left_bit = 6,
	_camera_control_roll_right_bit = 7,
	NUMBER_OF_CAMERA_CONTROL_BITS = 8,
};

#endif /* CAMERA_CONTROL_FLAGS_H */

// camera_control_flags: DB-verified via types_enum_values _9CC0AEABA6364E542FF927C4C97AED56
