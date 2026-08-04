#ifndef CONVERSATION_LINE_FLAGS_H
#define CONVERSATION_LINE_FLAGS_H

/* conversation_line_flags — conversation_line flags.
 * DB-verified: types_enum_values _3E20C878553DFA759413AA0284D3000F carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum conversation_line_flags {
	_ai_conversation_line_addressee_look_back_bit = 0,
	_ai_conversation_line_everyone_look_at_speaker_bit = 1,
	_ai_conversation_line_everyone_look_at_addressee_bit = 2,
	_ai_conversation_line_wait_after_until_told_to_advance_bit = 3,
	_ai_conversation_line_wait_until_speaker_nearby_bit = 4,
	_ai_conversation_line_wait_until_everyone_nearby_bit = 5,
	NUMBER_OF_CONVERSATION_LINE_FLAGS = 6,
};

#endif /* CONVERSATION_LINE_FLAGS_H */

// conversation_line_flags: DB-verified via types_enum_values _3E20C878553DFA759413AA0284D3000F
