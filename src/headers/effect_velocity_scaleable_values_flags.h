#ifndef EFFECT_VELOCITY_SCALEABLE_VALUES_FLAGS_H
#define EFFECT_VELOCITY_SCALEABLE_VALUES_FLAGS_H

/* effect_velocity_scaleable_values_flags — effect_velocity_scaleable_values flags.
 * DB-verified: types_enum_values _1694A43D5780EA11C3B2605EF9409211 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum effect_velocity_scaleable_values_flags {
	_effect_velocity_bit = 0,
	_effect_velocity_delta_bit = 1,
	_effect_velocity_cone_bit = 2,
	_effect_angular_velocity_bit = 3,
	_effect_angular_velocity_delta_bit = 4,
	NUMBER_OF_EFFECT_VELOCITY_SCALEABLE_VALUES = 5,
};

#endif /* EFFECT_VELOCITY_SCALEABLE_VALUES_FLAGS_H */

// effect_velocity_scaleable_values_flags: DB-verified via types_enum_values _1694A43D5780EA11C3B2605EF9409211
