#ifndef AI_ATOM_ACTION_MODIFIER_H
#define AI_ATOM_ACTION_MODIFIER_H

/* ai_atom_action_modifier — atom_modifier values for the _ai_atom_action command atom.
 * DB-verified via types_enum_values $40752B1275C820ACC6DEE8000929E049. */
enum ai_atom_action_modifier {
	_ai_atom_action_modifier_berserk = 0,
	_ai_atom_action_modifier_surprise_front = 1,
	_ai_atom_action_modifier_surprise_back = 2,
	_ai_atom_action_modifier_evade_left = 3,
	_ai_atom_action_modifier_evade_right = 4,
	_ai_atom_action_modifier_dive_forward = 5,
	_ai_atom_action_modifier_dive_back = 6,
	_ai_atom_action_modifier_dive_left = 7,
	_ai_atom_action_modifier_dive_right = 8,
	_ai_atom_action_modifier_vehicle_woohoo = 9,
	_ai_atom_action_modifier_vehicle_scared = 10,
	NUMBER_OF_AI_ATOM_ACTION_MODIFIERS = 11,
};

#endif /* AI_ATOM_ACTION_MODIFIER_H */
