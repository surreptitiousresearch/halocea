#ifndef LOOPING_SOUND_TRACK_FLAGS_H
#define LOOPING_SOUND_TRACK_FLAGS_H

/* looping_sound_track_flags — looping_sound_track.flags (unsigned int @0).
 * DB-verified: types_enum_values _8BE6820743FACC5FA6079BFCE63B47A8 (verbatim).
 * Values are BIT INDICES — use as (1u << _bit). */
enum looping_sound_track_flags {
	_fade_in_at_start_bit = 0,
	_fade_out_at_stop_bit = 1,
	_fade_in_alternate_bit = 2,
	NUMBER_OF_LOOPING_SOUND_TRACK_FLAGS = 3,
};

#endif /* LOOPING_SOUND_TRACK_FLAGS_H */
