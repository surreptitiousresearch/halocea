#ifndef RACE_MESSAGE_H
#define RACE_MESSAGE_H

/* race_message — race-engine extension of the game_engine_message HUD/score-message id space.
 * Ids continue past game_engine_message_count (31); the score-message dispatch (e.g.
 * race_engine_display_score) switches over game_engine_message base ids (0-30) plus these.
 * DB-verified: types_enum_values _5216B533D6E7BE4A164D69F017647992 carries these values verbatim. */
enum race_message {
	race_message_you_completed_a_lap        = 31,
	race_message_ally_completed_a_lap       = 32,
	race_message_enemy_completed_a_lap      = 33,
	race_message_you_touched_a_flag_rally   = 34,
	race_message_ally_touched_a_flag_rally  = 35,
	race_message_enemy_touched_a_flag_rally = 36,
	race_message_new_best_lap_time          = 37,
};

#endif /* RACE_MESSAGE_H */
