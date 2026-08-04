#ifndef UNIT_ADD_WEAPON_MODE_H
#define UNIT_ADD_WEAPON_MODE_H

/* Mode argument of unit_add_weapon_to_inventory.
 * DB-verified via types_enum_values _7AB06D1A527E9E8FD4A21031966F9533 (names verbatim). */
enum unit_add_weapon_mode {
	_unit_add_weapon_normally = 0,
	_unit_add_weapon_as_current_weapon = 1,
	_unit_add_weapon_as_only_weapon = 2,
	NUMBER_OF_UNIT_ADD_WEAPON_FLAGS = 3,
};

#endif /* UNIT_ADD_WEAPON_MODE_H */
