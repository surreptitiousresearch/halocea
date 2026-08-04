#pragma once
/* GUESS: enum recovered by reconciliation, no ground-truth source — rationale: headers_ref names the
 * per-atom modifier enums (_ai_atom_crouch_modifier_* $B691CD17, _ai_atom_animation_mode_modifier_*
 * $BBBAAF2D, _actor_atom_grenade_modifier_* $DD5E2741, _ai_atom_move_immediate_modifier_* $F82EA65E)
 * but not the atom opcode enum itself. The names below follow that established _ai_atom_* naming and
 * are pinned by the handlers in action_obey_command_begin/perform/end.c: 5 consumes the
 * animation-mode modifiers, 6 the crouch modifiers, 8 the grenade modifiers, and 0x16 the
 * move-immediate direction modifiers; 0 is a pure pause_timer wait, 0xC wakes an hs script,
 * 0xD plays a user animation, 0xE a recorded animation, 0x10 speaks a vocalization, 0x12 sets
 * obey initiative, 0x14 jumps (loops) to another command, 0x15 kills the unit via act-of-god
 * damage flags, 0x17-0x19 are the look-random/look-player/look-object variants of 4, 0x1A sets
 * the destination radius and 0x1B teleports. */

/* layout bound to DB enum: types_enum_values _4611C5E6C1A0304C2CFDEDFC63D5CCD1 */
enum ai_atom_type {
	_ai_atom_pause = 0x0,
	_ai_atom_go_to = 0x1,
	_ai_atom_go_to_and_face = 0x2,
	_ai_atom_move_direction = 0x3,
	_ai_atom_look = 0x4,
	_ai_atom_animation_mode = 0x5,
	_ai_atom_crouch = 0x6,
	_ai_atom_shoot = 0x7,
	_ai_atom_grenade = 0x8,
	_ai_atom_vehicle = 0x9,
	_ai_atom_running_jump = 0xA,
	_ai_atom_targeted_jump = 0xB,
	_ai_atom_script = 0xC,
	_ai_atom_animate = 0xD,
	_ai_atom_recording = 0xE,
	_ai_atom_action = 0xF,
	_ai_atom_vocalize = 0x10,
	_ai_atom_targeting = 0x11,
	_ai_atom_initiative = 0x12,
	_ai_atom_wait = 0x13,
	_ai_atom_loop = 0x14,
	_ai_atom_die = 0x15,
	_ai_atom_move_immediate = 0x16,
	_ai_atom_look_random = 0x17,
	_ai_atom_look_player = 0x18,
	_ai_atom_look_object = 0x19,
	_ai_atom_set_radius = 0x1A,
	_ai_atom_teleport = 0x1B,
	NUMBER_OF_AI_ATOM_TYPES = 0x1C,
};
