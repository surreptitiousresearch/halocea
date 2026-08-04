#pragma once

/* player_respawn_failure — reason a dead multiplayer player cannot respawn yet
 * (players_get_respawn_failure return value).
 * Source: reference enum _FCAF0CCAC9FEB581C2EB5A01B4D17824 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum player_respawn_failure {
	_player_respawn_failure_none = 0,
	_player_respawn_failure_combat = 1,
	_player_respawn_failure_enemies = 2,
	_player_respawn_failure_moving = 3,
	_player_respawn_failure_vehicle = 4,
	NUMBER_OF_PLAYER_RESPAWN_FAILURES = 5,
};

// player_respawn_failure: DB-verified via types_enum_values _FCAF0CCAC9FEB581C2EB5A01B4D17824 (anonymous compiled enum, names sans leading _)
