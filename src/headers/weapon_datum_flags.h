#pragma once

/* Bit indices for the runtime _weapon_datum.flags (unsigned int @0x00). DB-AUTHORITATIVE: the enumerator
 * names are bound verbatim to DB enum _74BB078C12A9FF18BFCCEAAC69A4487A (see line below). Use as
 * (1u << _bit). This is the RUNTIME weapon-object flags word, distinct from the tag-level
 * _weapon_definition.flags (weapon_definition_flags.h). The evidence notes below predate the DB
 * extraction and describe observed runtime behaviour for the lower bits; the names are now DB-confirmed.
 *
 * Evidence per bit:
 *   0 overheated        — set when heat >= heat_overheated_threshold, cleared when heat drops below
 *                         heat_recovery_threshold. STRONG (fully proven here).
 *   1 venting           — set during cooldown once heat is within one frame of heat_recovery_threshold,
 *                         cleared together with bit 0 when venting completes. GUESS label.
 *   2 super_recoil_pending — tested (with weapon_type == 3) to emit the "overheating super recoil"
 *                         first-person message, then cleared. Set via a path outside the reversed set. GUESS label.
 *   3 reload_requested  — set when the reload control input (0x8) is latched for an authoritative
 *                         weapon with magazines, consumed by weapon_magazine_start_reload. STRONG. */
/* layout bound to DB enum: types_enum_values _74BB078C12A9FF18BFCCEAAC69A4487A */
enum weapon_datum_flags
{
	_weapon_overheated_bit = 0x0,
	_weapon_overheated_exit_bit = 0x1,
	_weapon_overheat_recoil_bit = 0x2,
	_weapon_needs_to_reload_bit = 0x3,
	_weapon_integrated_light_on_bit = 0x4,
	_weapon_multiplayer_inventory_flag = 0x5,
	_weapon_multiplayer_flag = 0x6,
	NUMBER_OF_WEAPON_FLAGS = 0x7,
};
