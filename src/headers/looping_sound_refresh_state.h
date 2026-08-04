#ifndef LOOPING_SOUND_REFRESH_STATE_H
#define LOOPING_SOUND_REFRESH_STATE_H

/* looping_sound_refresh_state - values for looping_sound_datum.state.
 * DB-verified: types_enum_values _824D65BA9048C738BBF1E96C7327B9BC (verbatim). */
enum looping_sound_refresh_state {
	_looping_sound_refresh_start = 0,
	_looping_sound_refresh_loop = 1,
	_looping_sound_refresh_stop = 2,
	NUMBER_OF_LOOPING_SOUND_REFRESH_STATES = 3,
};

#endif /* LOOPING_SOUND_REFRESH_STATE_H */
