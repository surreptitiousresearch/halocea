#ifndef KING_HILL_STATE_H
#define KING_HILL_STATE_H

/* king_hill_state — King-of-the-Hill hill control state machine (king_globals.hill_state).
 * DB-verified: types_enum_values _B689804D8208CBCF7C869E7646015A25 carries these verbatim. */
enum king_hill_state {
	king_hill_uncontrolled = 0,
	king_hill_controlled = 1,
	king_hill_controlled_red = 2,
	king_hill_controlled_blue = 3,
	king_hill_contested = 4,
};

#endif /* KING_HILL_STATE_H */

// king_hill_state: DB-verified via types_enum_values _B689804D8208CBCF7C869E7646015A25
