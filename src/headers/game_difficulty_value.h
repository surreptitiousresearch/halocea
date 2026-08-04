#ifndef GAME_DIFFICULTY_VALUE_H
#define GAME_DIFFICULTY_VALUE_H

/* game_difficulty_value — the `value_type` selector passed to game_difficulty_get_team_value().
 * DB-verified via types_enum_values _6B2257578227D6142B9E4D04BE579629 (anonymous compiled enum;
 * NUMBER_OF_GAME_DIFFICULTY_VALUES = 35). Reproduced verbatim. */
enum game_difficulty_value {
	_game_difficulty_enemy_damage_scale = 0,
	_game_difficulty_enemy_vitality_scale = 1,
	_game_difficulty_enemy_shield_scale = 2,
	_game_difficulty_enemy_recharge_scale = 3,
	_game_difficulty_friend_damage_scale = 4,
	_game_difficulty_friend_vitality_scale = 5,
	_game_difficulty_friend_shield_scale = 6,
	_game_difficulty_friend_recharge_scale = 7,
	_game_difficulty_infection_form_toughness = 8,
	_game_difficulty_health_unused6 = 9,
	_game_difficulty_rate_of_fire_scale = 10,
	_game_difficulty_fire_projectile_error_scale = 11,
	_game_difficulty_burst_error_scale = 12,
	_game_difficulty_new_target_delay_scale = 13,
	_game_difficulty_burst_separation_delay_scale = 14,
	_game_difficulty_target_tracking_bonus = 15,
	_game_difficulty_target_leading_bonus = 16,
	_game_difficulty_overcharge_chance_scale = 17,
	_game_difficulty_special_fire_delay_scale = 18,
	_game_difficulty_projectile_guidance_vs_player_scale = 19,
	_game_difficulty_melee_delay_bonus = 20,
	_game_difficulty_melee_delay_scale = 21,
	_game_difficulty_fire_unused6 = 22,
	_game_difficulty_grenade_chance_scale = 23,
	_game_difficulty_grenade_timer_scale = 24,
	_game_difficulty_grenade_unused1 = 25,
	_game_difficulty_grenade_unused2 = 26,
	_game_difficulty_grenade_unused3 = 27,
	_game_difficulty_major_normal_placement = 28,
	_game_difficulty_major_few_placement = 29,
	_game_difficulty_major_many_placement = 30,
	_game_difficulty_unused1 = 31,
	_game_difficulty_unused2 = 32,
	_game_difficulty_unused3 = 33,
	_game_difficulty_unused4 = 34,
	NUMBER_OF_GAME_DIFFICULTY_VALUES = 35,
};

#endif /* GAME_DIFFICULTY_VALUE_H */
