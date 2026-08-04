#pragma once
/* ai_information_type — the "information_type" argument of ai_communication_event
 * (stored into ai_information_packet.information_type; selects the information_data arm).
 * Source: DB types_enum_values _83F1EDC94AFF0E418537AD217C4B7877. */

enum ai_information_type {
	_ai_information_none = 0,
	_ai_information_allegiance = 1,
	_ai_information_combat_stimulus = 2,
	_ai_information_target_knowledge = 3,
	_ai_information_flee = 4,
	NUMBER_OF_AI_INFORMATION_TYPES = 5,
};

// ai_information_type: DB-verified via types_enum_values _83F1EDC94AFF0E418537AD217C4B7877 (anonymous compiled enum)
