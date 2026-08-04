#ifndef BIPED_DATUM_STATE_H
#define BIPED_DATUM_STATE_H

/* biped_datum_state — a biped's coarse movement state (_biped_datum.state, biped_state.state).
 * Source: reference enum _DEF844EF3F09529AA4D2083269A0CC9F (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. Named biped_datum_state here because src/headers/biped_state.h
 * already holds the unrelated biped_state snapshot struct. */
enum biped_datum_state {
	biped_state_idle = 0,
	biped_state_moving = 1,
	biped_state_unknown = 2,
	number_of_biped_states = 3
} ;

#endif /* BIPED_DATUM_STATE_H */

// biped_datum_state: DB-verified via types_enum_values _DEF844EF3F09529AA4D2083269A0CC9F (anonymous compiled enum, names sans leading _)
