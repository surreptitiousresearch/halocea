#ifndef EFFECT_PART_TYPE_SCALE_FLAGS_H
#define EFFECT_PART_TYPE_SCALE_FLAGS_H

/* effect_part_type_scale_flags - bit indices for the effect_part_type_scale.flags word.
 * DB-verified: types_enum_values _A557018F7F5B6840C5472C26DD742BF7 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum effect_part_type_scale_flags {
	_effect_part_type_scale_bit = 5,
	NUMBER_OF_EFFECT_PART_SCALEABLE_VALUES = 6,
};

#endif /* EFFECT_PART_TYPE_SCALE_FLAGS_H */

// effect_part_type_scale_flags: DB-verified via types_enum_values _A557018F7F5B6840C5472C26DD742BF7
