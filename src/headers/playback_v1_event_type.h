#ifndef PLAYBACK_V1_EVENT_TYPE_H
#define PLAYBACK_V1_EVENT_TYPE_H

/* playback_v1_event_type — animation playback (v1) event opcodes.
 * DB-verified: types_enum_values _555A8A31E472F572B87B738820999B14 carries these
 * named values verbatim. */
enum playback_v1_event_type {
	_playback_v1_nothing = 0,
	_playback_v1_end = 1,
	_playback_v1_animation_state_set = 2,
	_playback_v1_aiming_speed_set = 3,
	_playback_v1_control_flags_set = 4,
	_playback_v1_weapon_index_set = 5,
	_playback_v1_throttle_set = 6,
	_playback_v1_vectors_synchronize = 7,
	_playback_v1_vectors_desynchronize = 8,
	_playback_v1_facing_vector_set = 9,
	_playback_v1_aiming_vector_set = 10,
	_playback_v1_looking_vector_set = 11,
	_playback_v1_facing_aiming_vector_set = 12,
	_playback_v1_facing_looking_vector_set = 13,
	_playback_v1_aiming_looking_vector_set = 14,
	_playback_v1_facing_aiming_looking_vector_set = 15,
	_playback_v1_facing_angles_set = 16,
	_playback_v1_aiming_angles_set = 17,
	_playback_v1_looking_angles_set = 18,
	_playback_v1_facing_aiming_angles_set = 19,
	_playback_v1_facing_looking_angles_set = 20,
	_playback_v1_aiming_looking_angles_set = 21,
	_playback_v1_facing_aiming_looking_angles_set = 22,
	NUMBER_OF_PLAYBACK_V1_EVENTS = 23,
};

#endif /* PLAYBACK_V1_EVENT_TYPE_H */
