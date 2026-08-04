#ifndef SCENARIO_OBJECT_LOCATION_PLACEMENT_FLAGS_H
#define SCENARIO_OBJECT_LOCATION_PLACEMENT_FLAGS_H

/* scenario_object_location_placement_flags — scenario_object_location_placement flags.
 * DB-verified: types_enum_values _5DB9AEF73CCBFA199B29EFCB0C1D409B carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum scenario_object_location_placement_flags {
	_scenario_object_placement_not_automatic_bit = 0,
	_scenario_object_placement_not_on_easy_bit = 1,
	_scenario_object_placement_not_on_normal_bit = 2,
	_scenario_object_placement_not_on_hard_bit = 3,
	NUMBER_OF_SCENARIO_OBJECT_LOCATION_PLACEMENT_FLAGS = 4,
};

#endif /* SCENARIO_OBJECT_LOCATION_PLACEMENT_FLAGS_H */

// scenario_object_location_placement_flags: DB-verified via types_enum_values _5DB9AEF73CCBFA199B29EFCB0C1D409B
