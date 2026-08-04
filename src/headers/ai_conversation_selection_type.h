#pragma once
/* ai_conversation_selection_type — ai_conversation_participant.selection_type: how a
 * participant slot chooses its candidate actor. The DB enum
 * (_FE22E16109904F038D39F2F3157D0F6B) carries only NUMBER_OF_CONVERSATION_SELECTION_TYPES;
 * the individual constant names are recovered from headers_ref (IDA-exported). */

enum ai_conversation_selection_type {
	_ai_conversation_selection_friendly_actor = 0,
	_ai_conversation_selection_disembodied = 1,
	_ai_conversation_selection_in_player_vehicle = 2,
	_ai_conversation_selection_not_in_vehicle = 3,
	_ai_conversation_selection_sargeant = 4,
	_ai_conversation_selection_any_actor = 5,
	_ai_conversation_selection_radio = 6,
	_ai_conversation_selection_radio_sargeant = 7,
	NUMBER_OF_CONVERSATION_SELECTION_TYPES = 8,
};

// ai_conversation_selection_type: names from headers_ref; DB carries only NUMBER_OF (enum _FE22E16109904F038D39F2F3157D0F6B)
