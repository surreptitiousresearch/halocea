#ifndef COLLISION_SURFACE_FLAGS_H
#define COLLISION_SURFACE_FLAGS_H

/* collision_surface_flags — collision_surface flags.
 * DB-verified: types_enum_values _59F12195F5D391585F164C6C8D5C1B64 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum collision_surface_flags {
	_collision_surface_two_sided_bit = 0,
	_collision_surface_invisible_bit = 1,
	_collision_surface_climbable_bit = 2,
	_collision_surface_breakable_bit = 3,
	NUMBER_OF_COLLISION_SURFACE_FLAGS = 4,
};

#endif /* COLLISION_SURFACE_FLAGS_H */

// collision_surface_flags: DB-verified via types_enum_values _59F12195F5D391585F164C6C8D5C1B64
