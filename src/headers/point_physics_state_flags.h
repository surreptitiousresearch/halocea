#ifndef POINT_PHYSICS_STATE_FLAGS_H
#define POINT_PHYSICS_STATE_FLAGS_H

/* point_physics_state_flags — runtime state flags returned/consumed by point_physics_update
 * (distinct from the tag-side point_physics_definition flags and the point_physics_flags word).
 * DB-verified: types_enum_values _505539376C7924454D70AC4B98153419 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (flags >> _bit) & 1. */
enum point_physics_state_flags {
	_point_physics_in_air_bit = 0,
	_point_physics_in_water_bit = 1,
	_point_physics_collided_with_structure_bit = 2,
	_point_physics_collided_with_water_bit = 3,
	NUMBER_OF_POINT_PHYSICS_STATE_FLAGS = 4,
};

#endif /* POINT_PHYSICS_STATE_FLAGS_H */

// point_physics_state_flags: DB-verified via types_enum_values _505539376C7924454D70AC4B98153419
