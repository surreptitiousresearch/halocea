#ifndef UNIT_FLAGS_H
#define UNIT_FLAGS_H

/* unit_flags — bit indices for _unit_datum.flags (unsigned int).
 * Source: reference enum _C43855A431A811A70EBEC6F0749E7A54 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum unit_flags {
	_unit_actively_controlled_bit = 0x0,
	_unit_changed_aiming_angles_bit = 0x1,
	_unit_desired_weapon_invalid_bit = 0x2,
	_unit_exact_facing_bit = 0x3,
	_unit_active_camouflaged_bit = 0x4,
	_unit_super_camouflaged_bit = 0x5,
	_unit_controllable_bit = 0x6,
	_unit_ignore_hard_pings_bit = 0x7,
	_unit_must_set_up_dialogue_bit = 0x8,
	_unit_placed_here_dead_bit = 0x9,
	_unit_ignored_by_actors_bit = 0xA,
	_unit_preferred_target_bit = 0xB,
	_unit_no_falling_damage_bit = 0xC,
	_unit_feign_death_allowed_bit = 0xD,
	_unit_aim_without_turning_bit = 0xE,
	_unit_attached_melee_attack_bit = 0xF,
	_unit_not_enterable_by_player_bit = 0x10,
	_unit_spawned_actors_bit = 0x11,
	_unit_unloading_bit = 0x12,
	_unit_integrated_light_on_bit = 0x13,
	_unit_doesnt_drop_items_bit = 0x14,
	_unit_has_already_been_hacky_hacky_hacked_bit = 0x15,
	_unit_cannot_blink_bit = 0x16,
	_unit_impervious_bit = 0x17,
	_unit_suspended_bit = 0x18,
	_unit_running_blindly_bit = 0x19,
	_unit_integrated_night_vision_on_bit = 0x1A,
	_unit_possessed_by_recording_bit = 0x1B,
	_unit_desired_integrated_light_on_bit = 0x1C,
	_unit_desired_integrated_light_off_bit = 0x1D,
	NUMBER_OF_UNIT_DATUM_FLAGS = 0x1E,
};

#endif /* UNIT_FLAGS_H */

// unit_flags: DB-verified via types_enum_values _C43855A431A811A70EBEC6F0749E7A54 (anonymous compiled enum, names sans leading _)
