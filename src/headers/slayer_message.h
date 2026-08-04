#ifndef SLAYER_MESSAGE_H
#define SLAYER_MESSAGE_H

/* slayer game-engine score/status message codes passed to game_show_score_extended.
 * DB-verified: types_enum_values _BEF5A52FB7999D332E6F79B7A0A7A204 carries this value verbatim
 * (only slayer_message_new_target is emitted in the DB for this anonymous enum). */
enum slayer_message {
	slayer_message_new_target = 31,
};

#endif /* SLAYER_MESSAGE_H */
