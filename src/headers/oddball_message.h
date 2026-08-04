#ifndef ODDBALL_MESSAGE_H
#define ODDBALL_MESSAGE_H

/* oddball_message — Oddball game-engine HUD message ids (passed as `message`
 * to oddball_engine_display_score / oddball_engine game-engine message dispatch).
 * DB-verified: types_enum_values _530DA65350C92AFF0FCE485E5674216A carries these verbatim. */
enum oddball_message {
	oddball_message_you_have_the_ball = 31,
	oddball_message_ally_has_the_ball = 32,
	oddball_message_enemy_has_the_ball = 33,
	oddball_message_you_are_it = 34,
	oddball_message_ally_is_it = 35,
	oddball_message_enemy_is_it = 36,
	oddball_message_ball_was_reset = 37,
	oddball_message_enemy_has_the_ball_tick = 38,
	oddball_message_ally_has_the_ball_tick = 39,
	oddball_message_you_have_the_ball_tick = 40,
};

#endif /* ODDBALL_MESSAGE_H */

// oddball_message: DB-verified via types_enum_values _530DA65350C92AFF0FCE485E5674216A
