#ifndef CONVERSATION_PARTICIPANT_DEFINITION_FLAGS_H
#define CONVERSATION_PARTICIPANT_DEFINITION_FLAGS_H

/* conversation_participant_definition_flags — conversation_participant_definition flags.
 * DB-verified: types_enum_values _3D25CFE03D95466E548005C89FB72E69 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum conversation_participant_definition_flags {
	_ai_conversation_participant_optional_bit = 0,
	_ai_conversation_participant_has_alternate_bit = 1,
	_ai_conversation_participant_is_alternate_bit = 2,
	NUMBER_OF_CONVERSATION_PARTICIPANT_DEFINITION_FLAGS = 3,
};

#endif /* CONVERSATION_PARTICIPANT_DEFINITION_FLAGS_H */

// conversation_participant_definition_flags: DB-verified via types_enum_values _3D25CFE03D95466E548005C89FB72E69
