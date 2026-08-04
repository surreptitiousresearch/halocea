#ifndef SCENARIO_FLAGS_H
#define SCENARIO_FLAGS_H

/* scenario_flags - bit indices for the scenario.flags word.
 * DB-verified: types_enum_values _A0DD8FAE83DF7C24E09F03DD1B93327F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum scenario_flags {
	_scenario_cortana_hack_bit = 0,
	_scenario_demo_ui_bit = 1,
	NUMBER_OF_SCENARIO_FLAGS = 2,
};

#endif /* SCENARIO_FLAGS_H */

// scenario_flags: DB-verified via types_enum_values _A0DD8FAE83DF7C24E09F03DD1B93327F
