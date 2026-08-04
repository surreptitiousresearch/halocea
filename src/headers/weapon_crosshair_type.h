#pragma once
/* Bound to DB enum types_enum_values _008371B417146462A606B72B7A62E0C5 (2026-07-20 BLOCKED-retry). */

/* weapon_hud_crosshairs_element.crosshair_type / weapon_interface_state crosshair index. DB-AUTHORITATIVE
 * (was a GUESS before extraction). The DB compiled enum is named "crosshair_state"; canonical names below.
 * The 2 consumers (crosshairs_draw, hud_update_weapon_local_player) keep their prior identifiers as
 * same-value aliases. Prior guesses corrected by the DB at the never-proven indices: 3 was guessed
 * "should_reload" (DB flash_ammo), 7 "reload_overheat" (DB reload), 11 "should_reload_secondary_trigger"
 * (DB flash_secondary_ammo); the numeric values are unchanged so consumer dispatch is unaffected. */
enum weapon_crosshair_type {
	_crosshair_state_aim = 0,
	_weapon_crosshair_aim = 0,                                              /* legacy alias */
	_crosshair_state_zoom = 1,
	_weapon_crosshair_zoom_level = 1,                                       /* legacy alias */
	_crosshair_state_charge = 2,
	_weapon_crosshair_charge = 2,                                           /* legacy alias */
	_crosshair_state_flash_ammo = 3,
	_weapon_crosshair_should_reload = 3,                                    /* legacy alias (guessed name) */
	_crosshair_state_flash_heat = 4,
	_weapon_crosshair_flash_heat = 4,                                       /* legacy alias */
	_crosshair_state_flash_total_ammo = 5,
	_weapon_crosshair_flash_total_ammo = 5,                                 /* legacy alias */
	_crosshair_state_flash_total_battery = 6,
	_weapon_crosshair_flash_battery = 6,                                    /* legacy alias */
	_crosshair_state_reload = 7,
	_weapon_crosshair_reload_overheat = 7,                                  /* legacy alias (guessed name) */
	_crosshair_state_fired_with_no_ammo = 8,
	_weapon_crosshair_flash_when_firing_and_no_ammo = 8,                    /* legacy alias */
	_crosshair_state_threw_with_no_grenade = 9,
	_weapon_crosshair_flash_when_throwing_and_no_grenade = 9,               /* legacy alias */
	_crosshair_state_flash_ammo_none_for_reload = 10,
	_weapon_crosshair_low_ammo_and_none_left_to_reload = 10,                /* legacy alias */
	_crosshair_state_flash_secondary_ammo = 11,
	_weapon_crosshair_should_reload_secondary_trigger = 11,                 /* legacy alias (guessed name) */
	_crosshair_state_flash_secondary_total_ammo = 12,
	_weapon_crosshair_flash_secondary_total_ammo = 12,                      /* legacy alias */
	_crosshair_state_secondary_reload = 13,
	_weapon_crosshair_flash_secondary_reload = 13,                          /* legacy alias */
	_crosshair_state_fired_secondary_with_no_ammo = 14,
	_weapon_crosshair_flash_when_firing_secondary_trigger_with_no_ammo = 14,/* legacy alias */
	_crosshair_state_flash_secondary_ammo_none_for_reload = 15,
	_weapon_crosshair_low_secondary_ammo_and_none_left_to_reload = 15,      /* legacy alias */
	_crosshair_state_primary_trigger_ready = 16,
	_weapon_crosshair_primary_trigger_ready = 16,                           /* legacy alias */
	_crosshair_state_secondary_trigger_ready = 17,
	_weapon_crosshair_secondary_trigger_ready = 17,                         /* legacy alias */
	_crosshair_state_flash_fired_battery_depleted = 18,
	_weapon_crosshair_flash_when_firing_with_depleted_battery = 18,         /* legacy alias */
	NUMBER_OF_CROSSHAIR_STATES = 19,
	NUMBER_OF_WEAPON_CROSSHAIR_TYPES = 19,                                  /* legacy alias */
};
