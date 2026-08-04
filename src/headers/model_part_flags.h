#ifndef MODEL_PART_FLAGS_H
#define MODEL_PART_FLAGS_H

/* model_part_flags - bit indices for the model_part.flags word.
 * DB-verified: types_enum_values _DB5D8B7AED093C7D55E75D18207ECD12 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum model_part_flags {
	_model_part_stripped_bit = 0,
	_model_part_local_nodes = 1,
	NUMBER_OF_MODEL_GEOMETRY_PART_FLAGS = 2,
};

#endif /* MODEL_PART_FLAGS_H */

// model_part_flags: DB-verified via types_enum_values _DB5D8B7AED093C7D55E75D18207ECD12
