#pragma once
/* ai_communication_look_direction — value space of communication_possibility.recipient_look_direction
 * (ai_communication_event.c). Source: headers_ref _E67EDB6DFEA6CAD3CC439D5019D8597F. */

enum ai_communication_look_direction {
	_comm_look_direction_none = 0,
	_comm_look_direction_subject = 1,
	_comm_look_direction_protagonist = 2,
	_comm_look_direction_target = 3,
	_comm_look_direction_danger = 4,
	NUMBER_OF_AI_COMMUNICATION_LOOK_DIRECTIONS = 5,
};

// ai_communication_look_direction: DB-verified via types_enum_values _E67EDB6DFEA6CAD3CC439D5019D8597F (anonymous compiled enum, names sans leading _)
