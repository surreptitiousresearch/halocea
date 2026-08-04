#ifndef WEAPON_MOVEMENT_PENALTY_MODE_H
#define WEAPON_MOVEMENT_PENALTY_MODE_H

/* weapon_movement_penalty_mode — weapon_definition.movement_penalty_mode: when the held
 * weapon's movement penalty applies. */
enum weapon_movement_penalty_mode {
	_weapon_movement_penalty_always = 0,
	_weapon_movement_penalty_when_zoomed = 1,
	_weapon_movement_penalty_when_zoomed_or_reloading = 2,
	NUMBER_OF_WEAPON_MOVEMENT_PENALTY_MODES = 3,
};

#endif /* WEAPON_MOVEMENT_PENALTY_MODE_H */

// weapon_movement_penalty_mode: DB-verified via types_enum_values $C2D7C1751E2B615368304986DD815144
