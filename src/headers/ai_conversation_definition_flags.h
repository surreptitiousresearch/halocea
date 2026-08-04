#ifndef AI_CONVERSATION_DEFINITION_FLAGS_H
#define AI_CONVERSATION_DEFINITION_FLAGS_H

/* ai_conversation_definition_flags — bit indices for ai_conversation.flags.
 * Source: headers_ref _A3F7ADBC7864A7F7E4CD587B5B4B66A4 (verified).
 * Values are BIT INDICES — use as (1u << _bit). */
enum ai_conversation_definition_flags {
	_ai_conversation_stop_if_anyone_dies_bit = 0x0,
	_ai_conversation_stop_if_damaged_bit = 0x1,
	_ai_conversation_stop_if_visible_enemy_bit = 0x2,
	_ai_conversation_stop_if_alerted_to_enemy_bit = 0x3,
	_ai_conversation_player_must_be_visible_bit = 0x4,
	_ai_conversation_stop_other_actions_bit = 0x5,
	_ai_conversation_keep_trying_to_play_bit = 0x6,
	_ai_conversation_player_must_be_looking_at_bit = 0x7,
	NUMBER_OF_CONVERSATION_DEFINITION_FLAGS = 0x8,
};

#endif /* AI_CONVERSATION_DEFINITION_FLAGS_H */

// ai_conversation_definition_flags: DB-verified via types_enum_values _A3F7ADBC7864A7F7E4CD587B5B4B66A4 (anonymous compiled enum, names sans leading _)
