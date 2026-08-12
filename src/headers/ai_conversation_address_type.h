#pragma once
/* ai_conversation_address_type — the "address_type" of an ai_conversation_line: who the
 * speaker addresses/looks at. All four values are DB-named (corrected 2026-08-13, enum drain
 * Wave E: this header previously claimed only value 2 was carried in the DB — the oracle
 * names 0/1/3 as well, and the false negative blocked naming a live site).
 * Source: DB types_enum_values $709275E9D78A22A02A862FF2AF7D9E15 (members verbatim). */

enum ai_conversation_address_type {
	_ai_conversation_address_none = 0,
	_ai_conversation_address_player = 1,
	_ai_conversation_address_participant = 2,
	NUMBER_OF_CONVERSATION_ADDRESS_TYPES = 3,
};

// ai_conversation_address_type: DB-verified via types_enum_values $709275E9D78A22A02A862FF2AF7D9E15
