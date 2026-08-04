#ifndef SCRIPTED_EFFECT_FLAGS_H
#define SCRIPTED_EFFECT_FLAGS_H

/* scripted_effect_flags - bit indices for the scripted_effect.flags word.
 * DB-verified: types_enum_values _BC9E825301FF35B322C12610B9FCD15A carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum scripted_effect_flags {
	_scripted_effect_running_bit = 0,
	_scripted_effect_ending_bit = 1,
	NUMBER_OF_SCRIPTED_EFFECT_FLAGS = 2,
};

#endif /* SCRIPTED_EFFECT_FLAGS_H */

// scripted_effect_flags: DB-verified via types_enum_values _BC9E825301FF35B322C12610B9FCD15A
