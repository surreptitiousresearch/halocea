#pragma once

/* GUESS: bit indices for the runtime _weapon_datum.control_flags (unsigned __int16 @0x04). No ground
 * truth — DB and headers_ref type the field as a bare unsigned __int16 with no attached enum. These are
 * the per-frame trigger/reload control inputs the wielding unit forwards to the weapon; bits
 * reconstructed from how weapon_update (@0x836DD328) reads them. Use as (1u << _bit).
 *
 * Evidence per bit:
 *   1 primary_trigger   — drives trigger_should_fire[0]. STRONG.
 *   2 secondary_trigger — drives the secondary trigger want-to-fire (gated by the dual-wield tag flag).
 *                         STRONG.
 *   3 reload            — latches _weapon_needs_to_reload_bit for authoritative weapons. STRONG.
 *   4 inhibit_firing    — when set (or while state_timer is running) all triggers are forced not to
 *                         fire this frame. GUESS label (semantics proven, name inferred). */
/* layout bound to DB enum: types_enum_values _EF47EA7CA84B523F97F1E3F6B134941C */
enum weapon_control_flags
{
	_weapon_control_integrated_light_bit = 0x0,
	_weapon_control_primary_trigger_bit = 0x1,
	_weapon_control_secondary_trigger_bit = 0x2,
	_weapon_control_reload_bit = 0x3,
	_weapon_control_user_busy_bit = 0x4,
	_weapon_control_user_switching_weapons_bit = 0x5,
	_weapon_control_zoomed_bit = 0x6,
	NUMBER_OF_WEAPON_CONTROL_FLAGS = 0x7,
};
