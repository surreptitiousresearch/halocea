#ifndef OBJECT_DAMAGE_FLAGS_H
#define OBJECT_DAMAGE_FLAGS_H

/* object_damage_flags — bit indices for _object_datum.damage_flags (unsigned __int16 @ +0x102).
 * Source: reference enum _A883CDB10AE1B5BB01A02DE745769E63 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. Values are BIT INDICES — use as (1u << _object_dead_bit). */
enum object_damage_flags {
	_object_passed_body_damage_threshold_bit = 0x0,
	_object_passed_shield_damage_threshold_bit = 0x1,
	_object_dead_bit = 0x2,
	_object_shield_depleted_bit = 0x3,
	_object_shield_over_charging_bit = 0x4,
	_object_die_act_of_god_bit = 0x5,
	_object_die_act_of_god_silent_bit = 0x6,
	_object_melee_attack_inhibited_bit = 0x7,
	_object_ranged_attack_inhibited_bit = 0x8,
	_object_walking_inhibited_bit = 0x9,
	_object_cannot_hold_weapon_bit = 0xA,
	_object_cannot_take_damage_bit = 0xB,
	_object_shield_charging_bit = 0xC,
	_object_die_act_of_god_no_statistics_bit = 0xD,
	NUMBER_OF_OBJECT_DAMAGE_FLAGS = 0xE,
};

#endif /* OBJECT_DAMAGE_FLAGS_H */

// object_damage_flags: DB-verified via types_enum_values _A883CDB10AE1B5BB01A02DE745769E63 (anonymous compiled enum, names sans leading _)
