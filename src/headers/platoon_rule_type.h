#pragma once
/* platoon_rule_type — value space of platoon_rule.rule_type (encounter_test_rule.c).
 * Source: headers_ref _FD3209378D07BEB394028D569DC8613B. */

enum platoon_rule_type {
	_platoon_rule_never = 0,
	_platoon_rule_75_strength = 1,
	_platoon_rule_50_strength = 2,
	_platoon_rule_25_strength = 3,
	_platoon_rule_anybody_dead = 4,
	_platoon_rule_25_dead = 5,
	_platoon_rule_50_dead = 6,
	_platoon_rule_75_dead = 7,
	_platoon_rule_all_but_one_dead = 8,
	_platoon_rule_all_dead = 9,
	NUMBER_OF_PLATOON_RULE_TYPES = 10,
};

// platoon_rule_type: DB-verified via types_enum_values _FD3209378D07BEB394028D569DC8613B (anonymous compiled enum, names sans leading _)
