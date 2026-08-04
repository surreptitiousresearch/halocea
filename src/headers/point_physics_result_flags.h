#ifndef POINT_PHYSICS_RESULT_FLAGS_H
#define POINT_PHYSICS_RESULT_FLAGS_H

/* point_physics_result_flags — point_physics_result flags.
 * DB-verified: types_enum_values _505539376C7924454D70AC4B98153419 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum point_physics_result_flags {
	_point_physics_in_air_bit = 0,
	_point_physics_in_water_bit = 1,
	_point_physics_collided_with_structure_bit = 2,
	_point_physics_collided_with_water_bit = 3,
	NUMBER_OF_POINT_PHYSICS_RESULT_FLAGS = 4,
};

#endif /* POINT_PHYSICS_RESULT_FLAGS_H */

// point_physics_result_flags: DB-verified via types_enum_values _505539376C7924454D70AC4B98153419
