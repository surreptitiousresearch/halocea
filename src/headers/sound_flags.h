#ifndef SOUND_FLAGS_H
#define SOUND_FLAGS_H

/* sound_flags — sound flags.
 * DB-verified: types_enum_values _45E9C2BD83C393F9545C3D27EC469AEE carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum sound_flags {
	_sound_delayed_bit = 0,
	_sound_cached_bit = 1,
	_sound_inaudible_bit = 2,
	_sound_waiting_for_cache_bit = 3,
	NUMBER_OF_SOUND_FLAGS = 4,
};

#endif /* SOUND_FLAGS_H */

// sound_flags: DB-verified via types_enum_values _45E9C2BD83C393F9545C3D27EC469AEE
