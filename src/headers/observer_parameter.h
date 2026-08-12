#pragma once

/* observer_parameter — the camera parameter groups of the observer integrator (observer.h) and the
 * counts derived from them. Group order matches the flat scalar overlays exactly: observer.positions[14]
 * / observer_command.parameters[14] stride by observer_parameter_real_counts {3,3,1,1,6} (summing to
 * NUMBER_OF_OBSERVER_REAL_PARAMETERS = 14, of which the first NUMBER_OF_OBSERVER_CARTESIAN_REALS = 8 are
 * the plain cartesian scalars), and observer_derivative.n[11] / the a..f coefficient rows stride by
 * observer_parameter_derivative_real_counts {3,3,1,1,3} (summing to NUMBER_OF_OBSERVER_REAL_VELOCITIES
 * = 11) — both arrays are declared [5] in blam_data_globals.h, i.e. NUMBER_OF_OBSERVER_PARAMETERS.
 * The observer_update_{positions,velocities,accelerations,polynomial,command}.c loops walk exactly this
 * domain, with group 4 (_observer_orientation) routed through observer_apply_rotational_displacement.
 * Several DB names share a value (a count and the group that starts there); all are kept verbatim.
 * Source: types_enum_values $E9C358CD2D517B0E7C7E455061CEDFBB (DB-verified). */
enum observer_parameter {
	_observer_focus_position = 0,
	_observer_focus_offset = 1,
	NUMBER_OF_OBSERVER_REAL3D_PARAMETERS = 2,
	_observer_focus_distance = 2,
	_observer_field_of_view = 3,
	NUMBER_OF_OBSERVER_CARTESIAN_PARAMETERS = 4,
	_observer_orientation = 4,
	NUMBER_OF_OBSERVER_PARAMETERS = 5,
	NUMBER_OF_OBSERVER_REAL1D_PARAMETERS = 2,
	NUMBER_OF_OBSERVER_POLAR_PARAMETERS = 1,
	NUMBER_OF_OBSERVER_CARTESIAN_REALS = 8,
	NUMBER_OF_OBSERVER_POLAR_REALS = 6,
	NUMBER_OF_OBSERVER_REAL_PARAMETERS = 14,
	NUMBER_OF_OBSERVER_CARTESIAN_VELOCITIES = 8,
	NUMBER_OF_OBSERVER_POLAR_VELOCITIES = 3,
	NUMBER_OF_OBSERVER_REAL_VELOCITIES = 11,
};

// observer_parameter: DB-verified via types_enum_values $E9C358CD2D517B0E7C7E455061CEDFBB
