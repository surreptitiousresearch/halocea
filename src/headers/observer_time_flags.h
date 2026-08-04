#ifndef OBSERVER_TIME_FLAGS_H
#define OBSERVER_TIME_FLAGS_H

/* observer_time_flags - bit indices for the observer_time.flags word.
 * DB-verified: types_enum_values _B6AA7818E0909CCDAF12CE9B427E6E65 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum observer_time_flags {
	_observer_time_valid_bit = 0,
	_observer_time_force_bit = 1,
	NUMBER_OF_OBSERVER_TIME_FLAGS = 2,
};

#endif /* OBSERVER_TIME_FLAGS_H */

// observer_time_flags: DB-verified via types_enum_values _B6AA7818E0909CCDAF12CE9B427E6E65
