#ifndef ACTOR_STARTING_LOCATION_FLAGS_H
#define ACTOR_STARTING_LOCATION_FLAGS_H

/* actor_starting_location_flags — actor_starting_location flags.
 * DB-verified: types_enum_values _2DB3D64BF6759327CA620BD7B9AB14E3 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum actor_starting_location_flags {
	_actor_starting_location_required_bit = 0,
	NUMBER_OF_ACTOR_STARTING_LOCATION_FLAGS = 1,
};

#endif /* ACTOR_STARTING_LOCATION_FLAGS_H */

// actor_starting_location_flags: DB-verified via types_enum_values _2DB3D64BF6759327CA620BD7B9AB14E3
