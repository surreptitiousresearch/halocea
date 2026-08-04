#ifndef POINT_PHYSICS_DEFINITION_FLAGS_H
#define POINT_PHYSICS_DEFINITION_FLAGS_H

/* point_physics_definition_flags — bit indices for point_physics_definition.flags.
 * DB-verified: types_enum_values _631502F48F3EF7C85DBB930D65A6948F carries these named
 * bit indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum point_physics_definition_flags {
	_point_physics_flamethrower_collision_bit = 0x0,
	_point_physics_structure_collisions_bit = 0x1,
	_point_physics_water_collisions_bit = 0x2,
	_point_physics_simple_wind_bit = 0x3,
	_point_physics_damped_wind_bit = 0x4,
	_point_physics_no_gravity_bit = 0x5,
	NUMBER_OF_POINT_PHYSICS_DEFINITION_FLAGS = 0x6,
	NUMBER_OF_POINT_PHYSICS_DEFINITION_VARIABLES = 0x6,
};

#endif /* POINT_PHYSICS_DEFINITION_FLAGS_H */

// point_physics_definition_flags: DB-verified via types_enum_values _631502F48F3EF7C85DBB930D65A6948F (anonymous compiled enum, names sans leading _)
