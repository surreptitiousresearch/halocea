#pragma once

/* effect_vector — which reference direction an effect marker orients its emitters to. The five
 * members are exactly the five rows of the effect_marker_names table (src/data/effect_marker_names.c
 * @0x8417E93C: "normal", "incident", "negative incident", "reflection", "gravity"), and
 * damage_effect_new_at_location (@0x836B21B8) builds its marker_names / marker_forwards /
 * marker_points arrays in that same positional order with a marker_count of 5. Source:
 * types_enum_values $90561574DEB36F444A8CE35822EFB263 (DB-verified). The count member is spelled
 * NUMBER_OF_EFFECT_MARKERS in the DB — the vectors ARE the markers — and is kept verbatim. */
enum effect_vector {
	_effect_vector_normal = 0,
	_effect_vector_incident = 1,
	_effect_vector_negative_incident = 2,
	_effect_vector_reflected = 3,
	_effect_vector_gravity = 4,
	NUMBER_OF_EFFECT_MARKERS = 5,
};

// effect_vector: DB-verified via types_enum_values $90561574DEB36F444A8CE35822EFB263
