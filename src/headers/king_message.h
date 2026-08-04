#ifndef KING_MESSAGE_H
#define KING_MESSAGE_H

/* king_message — King-of-the-Hill game-engine HUD message ids (passed as `message`
 * to king_engine_display_score / king_engine game-engine message dispatch).
 * DB-verified: types_enum_values _834580D2E8AA1CAD2A4258A9C0FFD5BE carries these verbatim. */
enum king_message {
	king_message_enemy_on_the_hill = 31,
	king_message_ally_on_the_hill = 32,
	king_message_you_are_on_the_hill = 33,
};

#endif /* KING_MESSAGE_H */

// king_message: DB-verified via types_enum_values _834580D2E8AA1CAD2A4258A9C0FFD5BE
