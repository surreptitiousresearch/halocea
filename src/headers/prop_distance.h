#pragma once

/* prop_distance — the value domain of prop_datum.quantized_distance (0x121), the bucketed
 * actor-to-prop range. prop_status_refresh @0x837DA400 is the producer and quantizes
 * prop->distance into exactly these five buckets by ascending world-unit thresholds
 * (< 1 -> 0, < 6 -> 1, < 10 -> 2, < 30 -> 3, else 4), which pins the ordering to the DB
 * names melee/near/middle/far/distant. Consumers compare against the same domain
 * (actor_action_handle_combat_selection's point-blank test <= 1, actor_perception_update's
 * >= 3, actor_look_compute_prop_interest's interest falloff switch).
 * Source: types_enum_values $44ADBAE443330A02246AB7D2120EBFC3 (DB-verified). */
enum prop_distance {
	_prop_distance_melee = 0,
	_prop_distance_near = 1,
	_prop_distance_middle = 2,
	_prop_distance_far = 3,
	_prop_distance_distant = 4,
	NUMBER_OF_PROP_DISTANCES = 5,
};

// prop_distance: DB-verified via types_enum_values $44ADBAE443330A02246AB7D2120EBFC3
