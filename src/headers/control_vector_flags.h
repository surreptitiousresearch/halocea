#ifndef CONTROL_VECTOR_FLAGS_H
#define CONTROL_VECTOR_FLAGS_H

/* control_vector_flags — control_vector flags.
 * DB-verified: types_enum_values _19A95FEE355DFC9E11E1A83B8F73B33F carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum control_vector_flags {
	_control_vector_facing_bit = 0,
	_control_vector_aiming_bit = 1,
	_control_vector_looking_bit = 2,
	NUMBER_OF_CONTROL_VECTORS = 3,
};

#endif /* CONTROL_VECTOR_FLAGS_H */

// control_vector_flags: DB-verified via types_enum_values _19A95FEE355DFC9E11E1A83B8F73B33F
