#pragma once
/* ai_communication_enemy_status — value space of dialogue_usage.required_enemy_status: which
 * enemy-awareness condition a dialogue row requires before it may be spoken. Indexes the
 * six-entry enemy_status_flags[] gate built in ai_communication_event
 * (`row->required_enemy_status != -1 && !enemy_status_flags[row->required_enemy_status]`);
 * -1 means "no requirement".
 * Source: DB types_enum_values _03CBB9AFF58DD47D9B31654E176E3E28 (anonymous compiled enum). */

enum ai_communication_enemy_status {
	_comm_enemy_never = 0,
	_comm_enemy_dead = 1,
	_comm_enemy_lost = 2,
	_comm_enemy_not_visible = 3,
	_comm_enemy_not_dangerous = 4,
	_comm_enemy_visible = 5,
	NUMBER_OF_COMMUNICATION_ENEMY_STATUS_TYPES = 6,
};

// ai_communication_enemy_status: DB-verified via types_enum_values _03CBB9AFF58DD47D9B31654E176E3E28
