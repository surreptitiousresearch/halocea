#ifndef POINT_FLAGS_H
#define POINT_FLAGS_H

/* point_flags — point flags.
 * DB-verified: types_enum_values _35EE1D98A1945FD2941159B669ED5A99 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum point_flags {
	_point_at_rest_bit = 0,
	_point_on_ground_bit = 1,
	_point_on_volatile_surface_bit = 2,
	_point_in_water_bit = 3,
	_point_antigraving_bit = 4,
	NUMBER_OF_POINT_FLAGS = 5,
};

#endif /* POINT_FLAGS_H */

// point_flags: DB-verified via types_enum_values _35EE1D98A1945FD2941159B669ED5A99
