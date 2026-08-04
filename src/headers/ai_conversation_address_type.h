#pragma once
/* ai_conversation_address_type — the "address_type" of an ai_conversation_line: who the
 * speaker addresses/looks at. Only value 2 (address a specific participant) is carried in
 * the DB; value 1 (the triggering player) is used in code but is not a named DB enumerant.
 * Source: DB types_enum_values _709275E9D78A22A02A862FF2AF7D9E15. */

enum ai_conversation_address_type {
	/* 0 = none, 1 = triggering player (not named in DB) */
	_ai_conversation_address_participant = 2,
	NUMBER_OF_CONVERSATION_ADDRESS_TYPES = 3,
};

// ai_conversation_address_type: DB-verified via types_enum_values _709275E9D78A22A02A862FF2AF7D9E15 (only value 2 named)
