#ifndef GRENADE_OVERLAY_ON_FLAGS_H
#define GRENADE_OVERLAY_ON_FLAGS_H

/* grenade_overlay_on_flags - bit indices for the grenade_overlay_on.flags word.
 * DB-verified: types_enum_values _7C8771DE250846369DB96210CAD89149 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum grenade_overlay_on_flags {
	_grenade_overlay_on_flashing_bit = 0,
	_grenade_overlay_on_empty_bit = 1,
	_grenade_overlay_on_default_bit = 2,
	_grenade_overlay_on_always_bit = 3,
	NUMBER_OF_GRENADE_OVERLAY_FLAGS = 4,
};

#endif /* GRENADE_OVERLAY_ON_FLAGS_H */

// grenade_overlay_on_flags: DB-verified via types_enum_values _7C8771DE250846369DB96210CAD89149
