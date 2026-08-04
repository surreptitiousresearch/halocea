#ifndef DAMAGE_DEFINITION_FLAGS_H
#define DAMAGE_DEFINITION_FLAGS_H

/* damage_definition_flags — bit indices for damage_definition.flags (unsigned int @0x4),
 * the jpt! damage block (tag data + 452).
 * Source: reference enum _EED22F2B7C269BABA036AF5AB0060476 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum damage_definition_flags {
	_damage_does_not_hurt_owner_bit = 0x0,
	_damage_can_cause_headshots_bit = 0x1,
	_damage_pings_resistant_units_bit = 0x2,
	_damage_does_not_hurt_friends_bit = 0x3,
	_damage_does_not_ping_units_bit = 0x4,
	_damage_detonates_explosives_bit = 0x5,
	_damage_only_hurts_shields_bit = 0x6,
	_damage_causes_flaming_death_bit = 0x7,
	_damage_draw_indicators_down_bit = 0x8,
	_damage_skips_shields_bit = 0x9,
	_damage_does_not_hurt_infection_forms_bit = 0xA,
	_damage_can_cause_multiplayer_headshots_bit = 0xB,
	_damage_infection_form_pop_bit = 0xC,
	NUMBER_OF_DAMAGE_DEFINITION_FLAGS = 0xD,
};

#endif /* DAMAGE_DEFINITION_FLAGS_H */

// damage_definition_flags: DB-verified via types_enum_values _EED22F2B7C269BABA036AF5AB0060476 (anonymous compiled enum, names sans leading _)
