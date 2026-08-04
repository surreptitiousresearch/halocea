#pragma once
/* ai_communication_priority — vocalization/communication priority ladder
 * (communication_possibility.communication_priority, ai_communication_started.c).
 * Source: headers_ref _D803A5283A249DB4A3B2CAA0531D6505. */

enum ai_communication_priority {
	_ai_communication_priority_none = 0,
	_ai_communication_priority_filler = 1,
	_ai_communication_priority_chatter = 2,
	_ai_communication_priority_talk = 3,
	_ai_communication_priority_communicate = 4,
	_ai_communication_priority_shout = 5,
	_ai_communication_priority_yell = 6,
	_ai_communication_priority_exclaim = 7,
	NUMBER_OF_AI_COMMUNICATION_PRIORITIES = 8,
};

// ai_communication_priority: DB-verified via types_enum_values _D803A5283A249DB4A3B2CAA0531D6505 (anonymous compiled enum, names sans leading _)
