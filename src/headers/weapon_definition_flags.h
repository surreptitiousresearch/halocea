#pragma once
/* Bound to DB enum types_enum_values _9376EEB6E50799EA48CD35C44A1768C2 (2026-07-20 BLOCKED-retry). */

/* _weapon_definition.flags @0x00. DB-AUTHORITATIVE (was a GUESS before extraction). The IDA DB now
 * carries the full compiled bit-index enum ($9376EEB6..., NUMBER_OF_WEAPON_DEFINITION_FLAGS = 16). The
 * three bits the reconstruction proved are all confirmed: bit 3 must_be_readied, bit 6
 * prevents_grenade_throwing, bit 9 prevents_melee_attack.
 *
 * The DB enumerators (_weapon_*_bit) are BIT INDICES. The pre-existing consumer identifiers
 * (_weapon_definition_flag_*) are MASKS (1 << index) and are retained as same-mask aliases so the
 * `flags & _weapon_definition_flag_*` sites are unaffected. */
enum weapon_definition_flags {
	_weapon_vertical_heat_display_bit = 0,
	_weapon_mutually_exclusive_triggers_bit = 1,
	_weapon_attacks_on_bump_bit = 2,
	_weapon_must_be_readied_bit = 3,
	_weapon_doesnt_count_toward_maximum_bit = 4,
	_weapon_aim_assists_only_when_zoomed_bit = 5,
	_weapon_prevents_grenade_throwing_bit = 6,
	_weapon_must_be_picked_up_bit = 7,
	_weapon_holds_triggers_when_dropped_bit = 8,
	_weapon_prevents_melee_attack_bit = 9,
	_weapon_detonates_when_dropped_bit = 10,
	_weapon_cannot_fire_at_maximum_age_bit = 11,
	_weapon_secondary_trigger_overrides_grenades_bit = 12,
	_weapon_does_not_depower_active_camo_bit = 13,
	_weapon_enables_unit_integrated_night_vision_bit = 14,
	_weapon_non_players_use_melee_damage_bit = 15,
	NUMBER_OF_WEAPON_DEFINITION_FLAGS = 16,
};

/* Legacy consumer masks (1 << bit) — kept for the existing `flags & _weapon_definition_flag_*` sites. */
enum weapon_definition_flag_masks {
	_weapon_definition_flag_vertical_heat_display     = 1 << 0,
	_weapon_definition_flag_mutually_exclusive_triggers = 1 << 1,
	_weapon_definition_flag_attacks_automatically_on_bump = 1 << 2,
	_weapon_definition_flag_must_be_readied           = 1 << 3, /* proven by weapon_must_be_readied; DB-confirmed */
	_weapon_definition_flag_doesnt_count_toward_maximum = 1 << 4,
	_weapon_definition_flag_aim_assist_only_when_zoomed = 1 << 5,
	_weapon_definition_flag_prevents_grenade_throwing = 1 << 6, /* proven by weapon_prevents_grenade_throwing; DB-confirmed */
	_weapon_definition_flag_prevents_melee_attack     = 1 << 9, /* proven by weapon_prevents_melee_attack; DB-confirmed */
};
