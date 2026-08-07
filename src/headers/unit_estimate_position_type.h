#ifndef UNIT_ESTIMATE_POSITION_TYPE_H
#define UNIT_ESTIMATE_POSITION_TYPE_H

/* Which unit position to estimate when computing sight/line-of-fire points
 * (biped_get_sight_position, firing_position_compute_line_of_sight). */
enum unit_estimate_position_type {
	_unit_estimate_none = 0,
	_unit_estimate_head_standing = 1,
	_unit_estimate_head_crouching = 2,
	_unit_estimate_gun_position = 3,
	NUMBER_OF_UNIT_ESTIMATE_POSITION_MODES = 4,
};

#endif /* UNIT_ESTIMATE_POSITION_TYPE_H */

// unit_estimate_position_type: DB-verified via types_enum_values $D335DCFD3D26B33304952B2B934E2DDA
