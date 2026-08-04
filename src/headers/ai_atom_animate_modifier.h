#ifndef AI_ATOM_ANIMATE_MODIFIER_H
#define AI_ATOM_ANIMATE_MODIFIER_H

/* ai_atom_animate_modifier — atom_modifier values for the _ai_atom_animate command atom.
 * DB-verified via types_enum_values $56E85606C407597982D70FAD1E066295. */
enum ai_atom_animate_modifier {
	_ai_atom_animate_modifier_relative_movement = 0,
	_ai_atom_animate_modifier_absolute_movement = 1,
	_ai_atom_animate_modifier_absolute_movement_no_collision = 2,
	_ai_atom_animate_modifier_no_interpolation_relative_movement = 3,
	_ai_atom_animate_modifier_no_interpolation_absolute_movement = 4,
	_ai_atom_animate_modifier_no_interpolation_absolute_movement_no_collision = 5,
	NUMBER_OF_AI_ATOM_ANIMATE_MODIFIERS = 6,
};

#endif /* AI_ATOM_ANIMATE_MODIFIER_H */
