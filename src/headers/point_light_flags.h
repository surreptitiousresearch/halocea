#ifndef POINT_LIGHT_FLAGS_H
#define POINT_LIGHT_FLAGS_H

/* point_light_flags — point_light flags.
 * DB-verified: types_enum_values _64CF702A33D7FAD1FC140B226B3CD4B0 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum point_light_flags {
	_point_light_dynamic_bit = 0,
	_point_light_connects_to_map_bit = 1,
	_point_light_connected_to_map_bit = 2,
	_point_light_attached_to_first_person_weapon_bit = 3,
	NUMBER_OF_POINT_LIGHT_FLAGS = 4,
};

#endif /* POINT_LIGHT_FLAGS_H */

// point_light_flags: DB-verified via types_enum_values _64CF702A33D7FAD1FC140B226B3CD4B0
