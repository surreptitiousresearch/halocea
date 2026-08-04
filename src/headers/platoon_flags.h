#ifndef PLATOON_FLAGS_H
#define PLATOON_FLAGS_H

/* platoon_flags - bit indices for the platoon.flags word.
 * DB-verified: types_enum_values _E04464303DBF33C749284D618A0A4D5C carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum platoon_flags {
	_platoon_flee_upon_maneuver_bit = 0,
	_platoon_advancing_maneuver_bit = 1,
	_platoon_initially_defending_bit = 2,
	NUMBER_OF_PLATOON_FLAGS = 3,
};

#endif /* PLATOON_FLAGS_H */

// platoon_flags: DB-verified via types_enum_values _E04464303DBF33C749284D618A0A4D5C
