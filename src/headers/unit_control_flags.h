#ifndef UNIT_CONTROL_FLAGS_H
#define UNIT_CONTROL_FLAGS_H

/* unit_control_flags — bit indices for unit control input flags (unit.control_flags / actor output.control_flags / input.unit_control_flags).
 * Source: reference enum _DAB55C4E1FF70DE369E731C396390DE5 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum unit_control_flags {
	_unit_control_crouch_modifier_bit = 0x0,
	_unit_control_jump_bit = 0x1,
	_unit_control_user_animation1_bit = 0x2,
	_unit_control_user_animation2_bit = 0x3,
	_unit_control_integrated_light_bit = 0x4,
	_unit_control_exact_facing_bit = 0x5,
	_unit_control_action_bit = 0x6,
	_unit_control_use_equipment_bit = 0x7,
	_unit_control_look_dont_turn_bit = 0x8,
	_unit_control_force_alert_bit = 0x9,
	_unit_control_weapon_reload_bit = 0xA,
	_unit_control_weapon_primary_trigger_bit = 0xB,
	_unit_control_weapon_secondary_trigger_bit = 0xC,
	_unit_control_throw_grenade_bit = 0xD,
	_unit_control_swap_weapons_bit = 0xE,
	NUMBER_OF_UNIT_CONTROL_FLAGS = 0xF,
	UNIT_CONTROL_DRIVER_MASK = 0x3F,
	UNIT_CONTROL_GUNNER_MASK = 0x7C00,
};

#endif /* UNIT_CONTROL_FLAGS_H */

// unit_control_flags: DB-verified via types_enum_values _DAB55C4E1FF70DE369E731C396390DE5 (anonymous compiled enum, names sans leading _)
