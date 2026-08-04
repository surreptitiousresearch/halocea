#ifndef UNIT_PLAY_SPEECH_TYPE_H
#define UNIT_PLAY_SPEECH_TYPE_H

/* How a unit speech item is (re)played relative to the speech queue (unit_speak). */
enum unit_play_speech_type {
	_unit_play_speech_none = 0,
	_unit_play_speech_queue = 1,
	_unit_play_speech_immediate = 2,
	_unit_play_speech_immediate_dequeue = 3,
};

#endif /* UNIT_PLAY_SPEECH_TYPE_H */

// unit_play_speech_type: DB-verified via types_enum_values $74C238EDE6A8AFF2667D262B84422D2F
