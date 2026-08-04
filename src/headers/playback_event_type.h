#ifndef PLAYBACK_EVENT_TYPE_H
#define PLAYBACK_EVENT_TYPE_H

/* playback_event_type — animation playback event opcodes (compressed-vector stream).
 * DB-verified: types_enum_values _CC7E57D4750C3C0E5F7CCA06CF486AB3 carries these
 * named values verbatim. The char/short difference events encode a low-3-bit
 * control-vector mask biased above their base opcode (7 / 15). */
enum playback_event_type {
	_playback_nothing = 0,
	_playback_end = 1,
	_playback_animation_state_set = 2,
	_playback_aiming_speed_set = 3,
	_playback_control_flags_set = 4,
	_playback_weapon_index_set = 5,
	_playback_throttle_set = 6,
	_playback_vector_char_difference_set = 7,
	_playback_vector_short_difference_set = 15,
	NUMBER_OF_PLAYBACK_EVENTS = 24,
};

#endif /* PLAYBACK_EVENT_TYPE_H */
