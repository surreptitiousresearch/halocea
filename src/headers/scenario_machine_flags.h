#ifndef SCENARIO_MACHINE_FLAGS_H
#define SCENARIO_MACHINE_FLAGS_H

/* scenario_machine_flags — scenario_machine flags.
 * DB-verified: types_enum_values _1976727B497DE3AFB3C4BB594D5C55BB carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum scenario_machine_flags {
	_scenario_machine_does_not_operate_automatically_bit = 0,
	_scenario_machine_one_sided_bit = 1,
	_scenario_machine_never_appears_locked_bit = 2,
	_scenario_machine_opened_by_melee_attack_bit = 3,
	NUMBER_OF_SCENARIO_MACHINE_FLAGS = 4,
};

#endif /* SCENARIO_MACHINE_FLAGS_H */

// scenario_machine_flags: DB-verified via types_enum_values _1976727B497DE3AFB3C4BB594D5C55BB
