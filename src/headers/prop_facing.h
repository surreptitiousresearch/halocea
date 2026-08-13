#pragma once

/* prop_facing — the value domain of prop_datum.quantized_facing, the bucketed target-facing
 * classification. prop_status_refresh @0x837DA400 is the producer and buckets the facing dot
 * product by DESCENDING cosine (> 0.9925 -> 0 direct, > 0.9063 -> 1 nearby, <= 0.0 -> 4
 * behind, <= 0.5 -> 3 peripheral, else 2 central), pinning the ordering to the DB names.
 * Sibling domain of prop_distance.h ($44ADBAE4) on prop_datum.quantized_distance — same
 * producer, same 5-bucket shape; consumers threshold-compare both side by side
 * (prop_status_refresh.c:472, actor_situation_update, actor_perception_update).
 * Source: types_enum_values $1310F34DA0482E982A39A24EB6FD6B7D (DB-verified, verbatim). */
enum prop_facing {
	_prop_facing_direct = 0,
	_prop_facing_nearby = 1,
	_prop_facing_central = 2,
	_prop_facing_peripheral = 3,
	_prop_facing_behind = 4,
	NUMBER_OF_PROP_FACINGS = 5,
};

// prop_facing: DB-verified via types_enum_values $1310F34DA0482E982A39A24EB6FD6B7D
