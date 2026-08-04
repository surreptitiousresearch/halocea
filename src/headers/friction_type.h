#ifndef FRICTION_TYPE_H
#define FRICTION_TYPE_H

/* Mass-point friction decomposition basis (model mass point "friction type"):
 * point (no basis), or decompose about the forward / left / up axis. */
enum friction_type {
	_friction_type_point = 0,
	_friction_type_forward = 1,
	_friction_type_left = 2,
	_friction_type_up = 3,
	NUMBER_OF_FRICTION_TYPES = 4,
};

#endif /* FRICTION_TYPE_H */

// friction_type: DB-verified via types_enum_values $6CBD6362950B8A30A45AF63061A6A1F4
