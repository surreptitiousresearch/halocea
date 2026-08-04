#ifndef DAMAGE_MATERIAL_HEAD_FLAGS_H
#define DAMAGE_MATERIAL_HEAD_FLAGS_H

/* damage_material_head_flags - bit indices for the damage_material_head.flags word.
 * DB-verified: types_enum_values _8C50A83B0772377389180A2108995576 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum damage_material_head_flags {
	_damage_material_head_bit = 0,
	NUMBER_OF_DAMAGE_MATERIAL_FLAGS = 1,
};

#endif /* DAMAGE_MATERIAL_HEAD_FLAGS_H */

// damage_material_head_flags: DB-verified via types_enum_values _8C50A83B0772377389180A2108995576
