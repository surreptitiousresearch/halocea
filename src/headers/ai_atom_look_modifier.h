#ifndef AI_ATOM_LOOK_MODIFIER_H
#define AI_ATOM_LOOK_MODIFIER_H

/* ai_atom_look_modifier — atom_modifier values for the _ai_atom_look* command atoms.
 * DB-verified via types_enum_values $54543F6BE5A01428BB904358B2BBEBEF. */
enum ai_atom_look_modifier {
	_ai_atom_look_modifier_idle_aim = 0,
	_ai_atom_look_modifier_idle_turn_around = 1,
	_ai_atom_look_modifier_idle_look = 2,
	_ai_atom_look_modifier_force_facing = 3,
	_ai_atom_look_modifier_force_aim_weapon = 4,
	NUMBER_OF_AI_ATOM_LOOK_MODIFIERS = 5,
};

#endif /* AI_ATOM_LOOK_MODIFIER_H */
