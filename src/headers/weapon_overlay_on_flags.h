#ifndef WEAPON_OVERLAY_ON_FLAGS_H
#define WEAPON_OVERLAY_ON_FLAGS_H

/* weapon_overlay_on_flags - bit indices for the weapon_overlay_on.flags word.
 * DB-verified: types_enum_values _E5CF5C58FA4B2835F3E1B6B9FEA63F1F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum weapon_overlay_on_flags {
	_weapon_overlay_on_flashing_bit = 0,
	_weapon_overlay_on_empty_bit = 1,
	_weapon_overlay_on_reload_bit = 2,
	_weapon_overlay_on_default_bit = 3,
	_weapon_overlay_on_always_bit = 4,
	NUMBER_OF_WEAPON_OVERLAY_TYPE_FLAGS = 5,
};

#endif /* WEAPON_OVERLAY_ON_FLAGS_H */

// weapon_overlay_on_flags: DB-verified via types_enum_values _E5CF5C58FA4B2835F3E1B6B9FEA63F1F
