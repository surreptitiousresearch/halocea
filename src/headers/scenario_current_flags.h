#ifndef SCENARIO_CURRENT_FLAGS_H
#define SCENARIO_CURRENT_FLAGS_H

/* scenario_current_flags - bit indices for the scenario_current.flags word.
 * DB-verified: types_enum_values _74948354759A74CEE13D323D93D82788 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum scenario_current_flags {
	_scenario_current_simple_bit = 0,
	_scenario_current_damped_bit = 1,
	_scenario_current_force_water_bit = 2,
	_scenario_current_force_no_water_bit = 3,
	NUMBER_OF_SCENARIO_GET_CURRENT_BITS = 4,
};

#endif /* SCENARIO_CURRENT_FLAGS_H */

// scenario_current_flags: DB-verified via types_enum_values _74948354759A74CEE13D323D93D82788
