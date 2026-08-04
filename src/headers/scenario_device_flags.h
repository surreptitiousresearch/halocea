#ifndef SCENARIO_DEVICE_FLAGS_H
#define SCENARIO_DEVICE_FLAGS_H

/* scenario_device_flags — scenario_device flags.
 * DB-verified: types_enum_values _4F847FC114BEB6455A00C6BC4DE47309 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum scenario_device_flags {
	_scenario_device_initially_open_bit = 0,
	_scenario_device_initially_off_bit = 1,
	_scenario_device_changes_only_once_bit = 2,
	_scenario_device_position_reversed_bit = 3,
	_scenario_device_not_usable_bit = 4,
	SCENARIO_DEVICE_DATUM_FLAGS = 5,
};

#endif /* SCENARIO_DEVICE_FLAGS_H */

// scenario_device_flags: DB-verified via types_enum_values _4F847FC114BEB6455A00C6BC4DE47309
