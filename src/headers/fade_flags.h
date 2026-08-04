#ifndef FADE_FLAGS_H
#define FADE_FLAGS_H

/* fade_flags - bit indices for the fade.flags word.
 * DB-verified: types_enum_values _8BE6820743FACC5FA6079BFCE63B47A8 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum fade_flags {
	_fade_in_at_start_bit = 0,
	_fade_out_at_stop_bit = 1,
	_fade_in_alternate_bit = 2,
	NUMBER_OF_LOOPING_SOUND_TRACK_FLAGS = 3,
};

#endif /* FADE_FLAGS_H */

// fade_flags: DB-verified via types_enum_values _8BE6820743FACC5FA6079BFCE63B47A8
