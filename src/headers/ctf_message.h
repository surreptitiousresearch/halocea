#ifndef CTF_MESSAGE_H
#define CTF_MESSAGE_H

/* ctf_message — CTF HUD "you/ally/enemy scored / flag state" message codes passed to
 * game_show_score_you_ally_enemy and friends.
 * DB-verified: types_enum_values _62D85788C02DFA0FAA98AD77B04A7384 carries these values verbatim
 * (the enum's lowest defined value is ctf_message_score = 31). */
enum ctf_message {
	ctf_message_score                        = 31,
	ctf_message_you_scored                   = 32,
	ctf_message_enemy_scored                 = 33,
	ctf_message_good_guys_scored             = 34,
	ctf_message_you_have_the_flag            = 35,
	ctf_message_you_returned_the_flag        = 36,
	ctf_message_enemy_has_the_flag           = 37,
	ctf_message_enemy_has_the_flag_tick      = 38,
	ctf_message_enemy_returned_the_flag      = 39,
	ctf_message_good_guys_have_the_flag      = 40,
	ctf_message_good_guys_returned_the_flag  = 41,
	ctf_message_your_flag_was_returned       = 42,
	ctf_message_enemy_flag_was_returned      = 43,
	ctf_message_time_expired                 = 44,
	ctf_message_you_are_on_attack            = 45,
	ctf_message_you_are_on_defense           = 46,
};

#endif /* CTF_MESSAGE_H */
