#ifndef SCENARIO_UNIT_DEAD_FLAGS_H
#define SCENARIO_UNIT_DEAD_FLAGS_H

/* scenario_unit_dead_flags - bit indices for the scenario_unit_dead.flags word.
 * DB-verified: types_enum_values _AF9D27FDF323E5083DE7F4EA3E935E9F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum scenario_unit_dead_flags {
	_scenario_unit_dead_bit = 0,
	NUMBER_OF_SCENARIO_UNIT_FLAGS = 1,
};

#endif /* SCENARIO_UNIT_DEAD_FLAGS_H */

// scenario_unit_dead_flags: DB-verified via types_enum_values _AF9D27FDF323E5083DE7F4EA3E935E9F
