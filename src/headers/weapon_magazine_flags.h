#ifndef WEAPON_MAGAZINE_FLAGS_H
#define WEAPON_MAGAZINE_FLAGS_H

/* weapon_magazine_flags — weapon_magazine flags.
 * DB-verified: types_enum_values _26FCA5AD982E42D155347E8981EE6C64 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum weapon_magazine_flags {
	_weapon_magazine_wastes_rounds_when_reloaded_bit = 0,
	_weapon_magazine_must_be_chambered_every_shot_bit = 1,
	NUMBER_OF_WEAPON_MAGAZINE_FLAGS = 2,
};

#endif /* WEAPON_MAGAZINE_FLAGS_H */

// weapon_magazine_flags: DB-verified via types_enum_values _26FCA5AD982E42D155347E8981EE6C64
