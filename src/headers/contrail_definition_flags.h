#ifndef CONTRAIL_DEFINITION_FLAGS_H
#define CONTRAIL_DEFINITION_FLAGS_H

/* contrail_definition_flags — bit indices for the contrail_definition flags word.
 * DB-verified: types_enum_values _06C9AE49C2C4EDD4CA4678F953783DD3 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum contrail_definition_flags {
	_contrail_first_point_unfaded_bit = 0,
	_contrail_last_point_unfaded_bit = 1,
	_contrail_points_start_on_media_bit = 2,
	_contrail_points_start_on_ground_bit = 3,
	_contrail_points_pinned_to_media_bit = 4,
	_contrail_points_pinned_to_ground_bit = 5,
	_contrail_fades_slowly_bit = 6,
	NUMBER_OF_CONTRAIL_DEFINITION_FLAGS = 7,
	CONTRAIL_POINTS_PINNED_FLAGS = 48,
};

#endif /* CONTRAIL_DEFINITION_FLAGS_H */

// contrail_definition_flags: DB-verified via types_enum_values _06C9AE49C2C4EDD4CA4678F953783DD3
