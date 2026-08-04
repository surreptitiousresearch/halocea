#ifndef OBJECT_FUNCTION_BOUNDS_MODE_H
#define OBJECT_FUNCTION_BOUNDS_MODE_H

/* object_function_bounds_mode — object-function `bounds_mode` field (how the evaluated output is
 * fitted to [lower_bound, upper_bound]). */
enum object_function_bounds_mode {
	_object_function_clip_to_bounds = 0,
	_object_function_clip_to_bounds_and_normalize = 1,
	_object_function_scale_to_fit_bounds = 2,
	NUMBER_OF_OBJECT_FUNCTION_BOUNDS_MODES = 3,
};

#endif /* OBJECT_FUNCTION_BOUNDS_MODE_H */

// object_function_bounds_mode: DB-verified via types_enum_values $202A49B7F1F7CC7398F588552465AA3A
