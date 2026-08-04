#ifndef FIND_ACTOR_FLAGS_H
#define FIND_ACTOR_FLAGS_H

/* find_actor_flags — find_actor flags.
 * DB-verified: types_enum_values _42C034AC25AF96A42295BE4025665843 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum find_actor_flags {
	_find_actor_allow_lookup_bit = 0,
	_find_actor_near_to_players_bit = 1,
	_find_actor_same_vehicle_bit = 2,
	_find_actor_allow_subject_bit = 3,
	_find_actor_allow_cause_bit = 4,
	NUMBER_OF_FIND_ACTOR_FLAGS = 5,
};

#endif /* FIND_ACTOR_FLAGS_H */

// find_actor_flags: DB-verified via types_enum_values _42C034AC25AF96A42295BE4025665843
