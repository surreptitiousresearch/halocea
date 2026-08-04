#ifndef WEAPON_TRIGGER_FLAGS_H
#define WEAPON_TRIGGER_FLAGS_H

/* weapon_trigger_flags — bit indices for the runtime weapon_trigger.flags (unsigned int @0x4).
 * Source: reference enum _E9DEAEC713E481B158029A13E7FFC2F0 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum weapon_trigger_flags {
	_weapon_trigger_released_since_last_shot_bit = 0x0,
	_weapon_trigger_was_down_bit = 0x1,
	_weapon_trigger_toggled_bit = 0x2,
	_weapon_trigger_useless_bit = 0x3,
	_weapon_trigger_blurred_bit = 0x4,
	_weapon_trigger_fired_before_charging_bit = 0x5,
	NUMBER_OF_WEAPON_TRIGGER_DATUM_FLAGS = 0x6,
};

#endif /* WEAPON_TRIGGER_FLAGS_H */

// weapon_trigger_flags: DB-verified via types_enum_values _E9DEAEC713E481B158029A13E7FFC2F0 (anonymous compiled enum, names sans leading _)
