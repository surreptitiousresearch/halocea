/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#ifndef UNIT_DEFINITION_FLAGS_H
#define UNIT_DEFINITION_FLAGS_H

/* unit_definition_flags — bit indices for unit_definition.unit.flags (the tag/definition side;
 * distinct from the runtime unit_flags $C43855A4). Use as (1u << _bit).
 *
 * GUESS (marked): no enum for this member survives in headers_ref or the IDA DB. The bit list is
 * the Halo CE editor ("Guerilla") unit tag flag convention, corroborated in this binary at two bits:
 *   - bit 8 "melee attack is fatal": unit_melee_attack_begin tests definition->unit.flags & 0x100
 *     and forces animation state 25 (_unit_state_dying) — the attacker dies.
 *   - bit 15 "melee attackers cannot attach": infection_swarm_control tests & 0x8000 on the HOST's
 *     unit definition when deciding whether an attached infection form may stay attached.
 * All other bit names are convention only, unverified in this binary. */
enum unit_definition_flags {
	_unit_definition_circular_aiming_bit = 0x0,
	_unit_definition_destroyed_after_dying_bit = 0x1,
	_unit_definition_half_speed_interpolation_bit = 0x2,
	_unit_definition_fires_from_camera_bit = 0x3,
	_unit_definition_entrance_inside_bounding_sphere_bit = 0x4,
	_unit_definition_unused_bit = 0x5,
	_unit_definition_causes_passenger_dialogue_bit = 0x6,
	_unit_definition_resists_pings_bit = 0x7,
	_unit_definition_melee_attack_is_fatal_bit = 0x8,
	_unit_definition_dont_reface_during_pings_bit = 0x9,
	_unit_definition_has_no_aiming_bit = 0xA,
	_unit_definition_simple_creature_bit = 0xB,
	_unit_definition_impact_melee_attaches_to_unit_bit = 0xC,
	_unit_definition_impact_melee_dies_on_shields_bit = 0xD,
	_unit_definition_cannot_open_doors_automatically_bit = 0xE,
	_unit_definition_melee_attackers_cannot_attach_bit = 0xF,
	/* High bits 16..24 use the DB-authoritative `_unit_*` names (types_enum_values
	 * _62D503FB72151F1243757361612DED81). */
	_unit_never_unsuspecting = 0x10,
	_unit_shield_sapping = 0x11,
	_unit_runs_around_flaming = 0x12,
	_unit_is_inconsequential = 0x13,
	_unit_is_special = 0x14,
	_unit_ignored_by_autoaiming = 0x15,
	_unit_shields_fry_infection_forms = 0x16,
	_unit_integrated_light_controls_weapon_directly = 0x17,
	_unit_integrated_light_fucking_lasts_forever = 0x18,
};

#endif /* UNIT_DEFINITION_FLAGS_H */

/* DB-verified via types_enum_values _62D503FB72151F1243757361612DED81. The DB carries 26 canonical
 * bits (0..24, NUMBER_OF_UNIT_DEFINITION_FLAGS = 25) with the `_unit_*` prefix; the adjudicated-KEEP
 * enum above (guessed `_unit_definition_*` names, bits 0..15) is retained for its consumers. DB names:
 *   0 _unit_has_circular_aiming_angles   1 _unit_is_destroyed_after_dying
 *   2 _unit_has_slow_animation_interpolation  3 _unit_fires_from_camera
 *   4 _unit_has_entrance_points_inside_bounding_sphere  5 _unit_unused
 *   6 _unit_causes_passenger_dialogue    7 _unit_resists_pings
 *   8 _unit_melee_attack_is_fatal        9 _unit_does_not_reorient_during_pings
 *  10 _unit_has_no_aiming               11 _unit_simple_creature
 *  12 _unit_impact_melee_attaches       13 _unit_impact_melee_die_on_shield
 *  14 _unit_cannot_open_doors_automatically  15 _unit_melee_attackers_cannot_attach
 *  16 _unit_never_unsuspecting          17 _unit_shield_sapping
 *  18 _unit_runs_around_flaming         19 _unit_is_inconsequential
 *  20 _unit_is_special                  21 _unit_ignored_by_autoaiming
 *  22 _unit_shields_fry_infection_forms 23 _unit_integrated_light_controls_weapon_directly
 *  24 _unit_integrated_light_fucking_lasts_forever */
