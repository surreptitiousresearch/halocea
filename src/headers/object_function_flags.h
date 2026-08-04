#ifndef OBJECT_FUNCTION_FLAGS_H
#define OBJECT_FUNCTION_FLAGS_H

/* object_function_flags - bit indices for the object_function.flags word.
 * DB-verified: types_enum_values _EA1AA69E0032D5711E8D8556ECF6F06F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum object_function_flags {
	_object_function_invert_function_bit = 0,
	_object_function_additive_bit = 1,
	_object_function_does_not_deactivate_below_lower_bound_bit = 2,
	NUMBER_OF_OBJECT_FUNCTION_DEFINITION_FLAGS = 3,
};

#endif /* OBJECT_FUNCTION_FLAGS_H */

// object_function_flags: DB-verified via types_enum_values _EA1AA69E0032D5711E8D8556ECF6F06F
