#pragma once
/* ai_communication_hostility — the "hostility" argument of ai_communication_event
 * (indexes the hostility_flags bitmap gate; dialogue_usage.required_hostility).
 * Source: DB types_enum_values _7283EB7EC8F2FEAD95688BFAD5825649. */

enum ai_communication_hostility {
	_comm_hostility_none = 0,
	_comm_hostility_self = 1,
	_comm_hostility_friend = 2,
	_comm_hostility_enemy = 3,
	_comm_hostility_traitor = 4,
	NUMBER_OF_AI_COMMUNICATION_HOSTILITIES = 5,
};

// ai_communication_hostility: DB-verified via types_enum_values _7283EB7EC8F2FEAD95688BFAD5825649 (anonymous compiled enum)
