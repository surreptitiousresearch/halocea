#pragma once
/* ai_communication_group — value space of dialogue_usage.required_group: which friend-proximity
 * group a dialogue row requires the protagonist to belong to. Indexes the two-entry
 * subject_groups[]/cause_groups[] gate built in ai_communication_event (extended = any
 * close-or-area friends, tactical = close friends specifically); -1 means "no requirement".
 * Source: DB types_enum_values _82CCC43A73911727B4C722EEE9E0D1CA (anonymous compiled enum). */

enum ai_communication_group {
	_comm_group_extended = 0,
	_comm_group_tactical = 1,
	NUMBER_OF_COMMUNICATION_GROUP_TYPES = 2,
};

// ai_communication_group: DB-verified via types_enum_values _82CCC43A73911727B4C722EEE9E0D1CA
