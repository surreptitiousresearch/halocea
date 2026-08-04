#ifndef SCENARIO_TYPE_H
#define SCENARIO_TYPE_H

/* scenario_type - global_scenario->type / cache_file_header.scenario_type. */
enum scenario_type {
	_scenario_type_solo = 0,
	_scenario_type_multiplayer = 1,
	_scenario_type_main_menu = 2,
	NUMBER_OF_SCENARIO_TYPES = 3,
};

#endif /* SCENARIO_TYPE_H */

// scenario_type: DB-verified via types_enum_values _7759FFB0F7BF0B97E9AE11278366D3D5 (anonymous compiled enum, names sans leading _)
