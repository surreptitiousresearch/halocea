#pragma once

/* ai_atom_animation_mode_modifier — atom_modifier values for the _ai_atom_animation_mode command
 * atom (ai_atom_type.h records that atom 5 consumes the animation-mode modifiers).
 * action_obey_command_begin.c takes command->atom_modifier in that case, rejects it when it is
 * >= 4 (== NUMBER_OF_AI_ATOM_ANIMATION_MODE_MODIFIERS below) and stores it into
 * obey_individual_complex_control.override_movement_type. This is the script-side domain and is
 * NOT actor_movement_type: the runtime enum carries a fifth value (_flaming), which is why the
 * bound is 4 and not 5. Source: types_enum_values $BBBAAF2DC17BAEBEA86C3F2029111C86 (DB-verified). */
enum ai_atom_animation_mode_modifier {
	_ai_atom_animation_mode_modifier_noncombat = 0,
	_ai_atom_animation_mode_modifier_asleep = 1,
	_ai_atom_animation_mode_modifier_combat = 2,
	_ai_atom_animation_mode_modifier_panic = 3,
	NUMBER_OF_AI_ATOM_ANIMATION_MODE_MODIFIERS = 4,
};

// ai_atom_animation_mode_modifier: DB-verified via types_enum_values $BBBAAF2DC17BAEBEA86C3F2029111C86 (anonymous compiled enum, names verbatim)
