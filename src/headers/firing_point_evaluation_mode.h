#ifndef FIRING_POINT_EVALUATION_MODE_H
#define FIRING_POINT_EVALUATION_MODE_H

/* firing_point_evaluation_mode - firing-position evaluation_mode selector. */
enum firing_point_evaluation_mode {
	_firing_point_evaluation_mode_fight = 0,
	_firing_point_evaluation_mode_panic = 1,
	_firing_point_evaluation_mode_cover = 2,
	_firing_point_evaluation_mode_uncover = 3,
	_firing_point_evaluation_mode_guard = 4,
	_firing_point_evaluation_mode_pursue = 5,
	_firing_point_evaluation_mode_avoid = 6,
	NUMBER_OF_FIRING_POINT_EVALUATION_MODES = 7,
};

#endif /* FIRING_POINT_EVALUATION_MODE_H */

// firing_point_evaluation_mode: DB-verified via types_enum_values _64A99E819DDFFEE0D8F235E999638A12 (anonymous compiled enum, names sans leading _)
