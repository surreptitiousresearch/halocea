#pragma once
/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _270498BB874CAD5ECABAECA7DA81ECAE exactly (DB uses the _equipment_powerup_* prefix; the _powerup_type_*
 * identifiers below are the consumer-facing names and are kept). */

/* GUESS: enum recovered by reconciliation, no ground-truth source — rationale: equipment_definition.
 * equipment.powerup_type dispatch in hud_add_item_message_from_network @0x836A5A78 plays
 * player_over_shield_screen_effect (and sets _object_shield_over_charging_bit) for 2,
 * player_active_camo_screen_effect for 3, player_health_pack_screen_effect for 5 — matching the classic
 * Halo CE equipment tag "powerup type" ordering (none, double speed, over shield, active camouflage,
 * full-spectrum vision, health, grenade), which reconciles every observed case. */
enum equipment_powerup_type {
	_powerup_type_none = 0,
	_powerup_type_double_speed = 1,
	_powerup_type_over_shield = 2,
	_powerup_type_active_camouflage = 3,
	_powerup_type_full_spectrum_vision = 4,
	_powerup_type_health = 5,
	_powerup_type_grenade = 6,
	NUMBER_OF_POWERUP_TYPES = 7,
};
