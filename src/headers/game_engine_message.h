#ifndef GAME_ENGINE_MESSAGE_H
#define GAME_ENGINE_MESSAGE_H

/* game_engine_message — multiplayer HUD state/notification message codes.
 * DB-verified: types_enum_values _98767E2EB1F0D516F9DAD25AB06E7A9B carries these values verbatim. */
enum game_engine_message {
	game_engine_message_welcome = 0,
	game_engine_message_killed_by_unknown = 1,
	game_engine_message_killed_by_biped = 2,
	game_engine_message_killed_by_vehicle = 3,
	game_engine_message_killed_by_player = 4,
	game_engine_message_killed_by_friendly_fire = 5,
	game_engine_message_killed_by_self = 6,
	game_engine_message_double_kill = 7,
	game_engine_message_killed_enemy = 8,
	game_engine_message_triple_kill = 9,
	game_engine_message_multi_kill = 10,
	game_engine_message_five_kills_in_row = 11,
	game_engine_message_ten_kills_in_a_row = 12,
	game_engine_message_killed_friendly = 13,
	game_engine_message_multi_kill_with_score = 14,
	game_engine_message_triple_kill_with_score = 15,
	game_engine_message_double_kill_with_score = 16,
	game_engine_message_ten_kills_in_a_row_with_score = 17,
	game_engine_message_five_kills_in_row_with_score = 18,
	game_engine_message_killed_enemy_with_score = 19,
	game_engine_message_winner = 20,
	game_engine_message_team_winner = 21,
	game_engine_message_show_score = 22,
	game_engine_message_odd_man_out = 23,
	game_engine_message_out_of_lives = 24,
	game_engine_message_respawn_timer = 25,
	game_engine_message_waiting_for_space_to_clear = 26,
	game_engine_message_player_quit_self = 27,
	game_engine_message_quit = 28,
	game_engine_message_press_back_for_score = 29,
	game_engine_message_time_left = 30,
	game_engine_message_count = 31,
};

#endif /* GAME_ENGINE_MESSAGE_H */
