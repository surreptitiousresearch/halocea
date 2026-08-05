#pragma once
/* post_combat_behavior_type — the four weighted behaviour buckets encounter_post_combat fills
 * (possibilities[4][2]) and selects from; the selected bucket index is then mapped to the actor's
 * external_orders.postcombat_type through global_post_combat_translation_table.
 * Source: DB types_enum_values _9254E6E76B1C0CAAA77312AB02264A98 (anonymous compiled enum). */

enum post_combat_behavior_type {
	_post_combat_behavior_shoot_target = 0,
	_post_combat_behavior_check_enemy = 1,
	_post_combat_behavior_check_friend = 2,
	_post_combat_behavior_celebrate = 3,
	NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES = 4,
};

// post_combat_behavior_type: DB-verified via types_enum_values _9254E6E76B1C0CAAA77312AB02264A98
