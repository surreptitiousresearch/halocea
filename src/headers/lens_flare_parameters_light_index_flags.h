#ifndef LENS_FLARE_PARAMETERS_LIGHT_INDEX_FLAGS_H
#define LENS_FLARE_PARAMETERS_LIGHT_INDEX_FLAGS_H

/* lens_flare_parameters_light_index_flags - bit indices for the lens_flare_parameters_light_index.flags word.
 * DB-verified: types_enum_values _8B02C1AD3F193EF5A5E8FF99B2B41208 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum lens_flare_parameters_light_index_flags {
	_lens_flare_parameters_light_index_structure_bit = 15,
	_lens_flare_parameters_light_index_mask = 32767,
};

#endif /* LENS_FLARE_PARAMETERS_LIGHT_INDEX_FLAGS_H */

// lens_flare_parameters_light_index_flags: DB-verified via types_enum_values _8B02C1AD3F193EF5A5E8FF99B2B41208
