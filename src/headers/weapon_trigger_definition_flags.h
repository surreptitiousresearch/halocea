#ifndef WEAPON_TRIGGER_DEFINITION_FLAGS_H
#define WEAPON_TRIGGER_DEFINITION_FLAGS_H

/* weapon_trigger_definition_flags — bit indices for weapon_trigger_definition.flags (unsigned int @0x0).
 * Source: reference enum _E803D98480427930E0DCD35DBC345473 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum weapon_trigger_definition_flags {
	_weapon_trigger_tracks_projectile_bit = 0x0,
	_weapon_trigger_random_firing_effects_bit = 0x1,
	_weapon_trigger_can_fire_with_partial_ammunition_bit = 0x2,
	_weapon_trigger_latched_bit = 0x3,
	_weapon_trigger_toggles_bit = 0x4,
	_weapon_trigger_uses_weapon_origin_bit = 0x5,
	_weapon_trigger_sticks_when_dropped_bit = 0x6,
	_weapon_trigger_ejection_port_during_chamber_animation_bit = 0x7,
	_weapon_trigger_discharging_spews_bit = 0x8,
	_weapon_trigger_analog_rate_of_fire_bit = 0x9,
	_weapon_trigger_use_error_when_unzoomed_bit = 0xA,
	_weapon_trigger_projectiles_cannot_be_aimed_bit = 0xB,
	_weapon_trigger_projectiles_have_identical_error_bit = 0xC,
	_weapon_trigger_client_side_only_bit = 0xD,
	NUMBER_OF_WEAPON_TRIGGER_FLAGS = 0xE,
};

#endif /* WEAPON_TRIGGER_DEFINITION_FLAGS_H */

// weapon_trigger_definition_flags: DB-verified via types_enum_values _E803D98480427930E0DCD35DBC345473 (anonymous compiled enum, names sans leading _)
