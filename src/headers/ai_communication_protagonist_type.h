#pragma once
/* ai_communication_protagonist_type — value space of dialogue_usage.protagonist_type and
 * reply_usage.protagonist_type: which participant of a communication event voices the line.
 * Source: types_enum_values _46D934B856F1F160A1356D8A13530989 (anonymous compiled enum, names sans leading _). */

enum ai_communication_protagonist_type {
	_comm_protagonist_subject = 0,
	_comm_protagonist_cause   = 1,
	_comm_protagonist_friend  = 2,
	_comm_protagonist_target  = 3,
	_comm_protagonist_enemy   = 4,
	NUMBER_OF_COMMUNICATION_PROTAGONIST_TYPES = 5,
};
