#ifndef MASS_POINT_METALLIC_FLAGS_H
#define MASS_POINT_METALLIC_FLAGS_H

/* mass_point_metallic_flags - bit indices for the mass_point_metallic.flags word.
 * DB-verified: types_enum_values _D1EEB471D146CC35EF6F8C5632211EA6 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum mass_point_metallic_flags {
	_mass_point_metallic_bit = 0,
	NUMBER_OF_MASS_POINT_DEFINITION_FLAGS = 1,
};

#endif /* MASS_POINT_METALLIC_FLAGS_H */

// mass_point_metallic_flags: DB-verified via types_enum_values _D1EEB471D146CC35EF6F8C5632211EA6
