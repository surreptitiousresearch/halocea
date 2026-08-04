#ifndef KEY_MODIFIER_FLAGS_H
#define KEY_MODIFIER_FLAGS_H

/* key_modifier_flags - bit indices for the key_modifier.flags word.
 * DB-verified: types_enum_values _ED117FA9516C7D53C32D0A817752E38B carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum key_modifier_flags {
	_key_modifier_shift_bit = 0,
	_key_modifier_control_bit = 1,
	_key_modifier_alt_bit = 2,
	NUMBER_OF_KEY_MODIFIER_FLAGS = 3,
};

#endif /* KEY_MODIFIER_FLAGS_H */

// key_modifier_flags: DB-verified via types_enum_values _ED117FA9516C7D53C32D0A817752E38B
