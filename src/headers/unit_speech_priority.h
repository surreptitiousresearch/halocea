#ifndef UNIT_SPEECH_PRIORITY_H
#define UNIT_SPEECH_PRIORITY_H

/* unit_speech_priority — priority class of a unit dialogue/vocalization request; passed as the
 * `priority`/`category` argument to unit_test_speech and stored in unit_speech_item.priority. Higher
 * priorities preempt lower ones in the unit's speech queue.
 * DB-verified via types_enum_values _33B1D83D7795DE5F5F8B60BD5E153189. */
enum unit_speech_priority {
	_unit_speech_none = 0,
	_unit_speech_idle = 1,
	_unit_speech_pain = 2,
	_unit_speech_talk = 3,
	_unit_speech_communicate = 4,
	_unit_speech_shout = 5,
	_unit_speech_scripted = 6,
	_unit_speech_involuntary = 7,
	_unit_speech_exclamation = 8,
	_unit_speech_scream = 9,
	_unit_speech_death = 10,
	NUMBER_OF_UNIT_SPEECH_PRIORITIES = 11,
};

#endif /* UNIT_SPEECH_PRIORITY_H */
