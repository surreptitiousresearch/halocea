#ifndef AI_COMMAND_LIST_FLAGS_H
#define AI_COMMAND_LIST_FLAGS_H

/* ai_command_list_flags — ai_command_list flags.
 * DB-verified: types_enum_values _3F0A7D85D5D4915A9813FB1248FE6688 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum ai_command_list_flags {
	_ai_command_list_allow_initiative_bit = 0,
	_ai_command_list_allow_targeting_bit = 1,
	_ai_command_list_disable_looking_bit = 2,
	_ai_command_list_disable_communication_bit = 3,
	_ai_command_list_disable_falling_damage_bit = 4,
	_ai_command_list_manual_structure_bsp_bit = 5,
	NUMBER_OF_AI_COMMAND_LIST_FLAGS = 6,
};

#endif /* AI_COMMAND_LIST_FLAGS_H */

// ai_command_list_flags: DB-verified via types_enum_values _3F0A7D85D5D4915A9813FB1248FE6688
