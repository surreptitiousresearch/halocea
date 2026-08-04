#pragma once

/* firing_position_group — index into squad_definition.firing_position_groups[8].
 * DB anonymous enum $E0033BFDC34B4A67572BA77DBDF1BE72. */
enum firing_position_group {
	_firing_position_group_attacking = 0,
	_firing_position_group_attacking_search = 1,
	_firing_position_group_attacking_guard = 2,
	_firing_position_group_defending = 3,
	_firing_position_group_defending_search = 4,
	_firing_position_group_defending_guard = 5,
	_firing_position_group_pursuing = 6,
	NUMBER_OF_FIRING_POSITION_GROUPS = 7,
	MAXIMUM_NUMBER_OF_FIRING_POSITION_GROUPS = 8,
};

// firing_position_group: DB-verified via types_enum_values $E0033BFDC34B4A67572BA77DBDF1BE72
