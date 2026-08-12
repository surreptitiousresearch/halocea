#pragma once

/* actor_atom_grenade_modifier — the atom_modifier domain of the _ai_atom_grenade command atom
 * (ai_atom_type.h, opcode 8): which grenade trajectory the obeying actor should plan. Consumed by
 * action_obey_command_begin.c, which range-checks command->atom_modifier and copies it into
 * complex_control->grenade_throw_trajectory_type (that field's own domain is the sibling family
 * _actor_grenade_trajectory_* $0D2BC86D625CAE783A25B28E06A51CC2, same 0/1/2 ordering).
 * Source: types_enum_values $DD5E274193EAC4E673A121F6FD9A6D44 (DB-verified). */
enum actor_atom_grenade_modifier {
	_actor_atom_grenade_modifier_toss = 0,
	_actor_atom_grenade_modifier_lob = 1,
	_actor_atom_grenade_modifier_bounce = 2,
	NUMBER_OF_AI_ATOM_GRENADE_MODIFIERS = 3,
};

// actor_atom_grenade_modifier: DB-verified via types_enum_values $DD5E274193EAC4E673A121F6FD9A6D44
