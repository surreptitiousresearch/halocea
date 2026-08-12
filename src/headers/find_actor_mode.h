#pragma once

/* find_actor_mode — the team-relation filter passed as the second argument of
 * ai_communication_find_global_actor_to_talk (@0x837CE578, whose own parameter is named
 * find_actor_mode): relative to team_index, 0 accepts only actors on the same team, 1 accepts any
 * actor that is not an enemy (the "friend" path), 2 accepts only enemies — the callee's own
 * game_team_is_enemy branch. The three AI-dialogue callers pass exactly 0/1/2 on their death,
 * _comm_protagonist_friend and _comm_protagonist_enemy branches. Source: types_enum_values
 * $937519FEBFF82740FC4524652418A653 (DB-verified). */
enum find_actor_mode {
	_find_actor_mode_same_team = 0,
	_find_actor_mode_friend = 1,
	_find_actor_mode_enemy = 2,
	NUMBER_OF_FIND_ACTOR_MODES = 3,
};

// find_actor_mode: DB-verified via types_enum_values $937519FEBFF82740FC4524652418A653
