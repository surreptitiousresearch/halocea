#ifndef STRUCTURE_MATERIAL_FLAGS_H
#define STRUCTURE_MATERIAL_FLAGS_H

/* structure_material_flags — bit indices for the structure_material flags word.
 * DB-verified: types_enum_values _0C8C01E2CCFC56DD16F0DAE9D178BB7C carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum structure_material_flags {
	_structure_material_coplanar_bit = 0,
	_structure_material_fog_plane_bit = 1,
	NUMBER_OF_STRUCTURE_MATERIAL_FLAGS = 2,
};

#endif /* STRUCTURE_MATERIAL_FLAGS_H */

// structure_material_flags: DB-verified via types_enum_values _0C8C01E2CCFC56DD16F0DAE9D178BB7C
