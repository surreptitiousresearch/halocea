#ifndef POINT_PHYSICS_FLAGS_H
#define POINT_PHYSICS_FLAGS_H

/* point_physics_flags — point_physics flags.
 * DB-verified: types_enum_values _3201517BF4BE36F48C0E4F249CF7F1AE carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum point_physics_flags {
	_point_physics_ignore_position_bit = 0,
	_point_physics_ignore_position_under_water_bit = 1,
	_point_physics_force_no_collisions_bit = 2,
	NUMBER_OF_POINT_PHYSICS_FLAGS = 3,
};

#endif /* POINT_PHYSICS_FLAGS_H */

// point_physics_flags: DB-verified via types_enum_values _3201517BF4BE36F48C0E4F249CF7F1AE
