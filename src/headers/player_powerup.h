#pragma once

/* player_powerup — the player-carried powerup states set by player_powerup_on/off.
 * Source: reference enum _7631F5BC00EB2EC7672D4CE153F5F765 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum player_powerup {
	_player_powerup_active_camouflage = 0,
	_player_powerup_full_spectrum_vision = 1,
	NUMBER_OF_PLAYER_POWERUPS = 2,
};

// player_powerup: DB-verified via types_enum_values _7631F5BC00EB2EC7672D4CE153F5F765 (anonymous compiled enum, names sans leading _)
