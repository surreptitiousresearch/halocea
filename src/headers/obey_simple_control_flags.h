/* Bound to DB enum types_enum_values _5F14168CA5C13897CF031FE97B7BC729 (2026-07-20 BLOCKED-retry). */
#ifndef OBEY_SIMPLE_CONTROL_FLAGS_H
#define OBEY_SIMPLE_CONTROL_FLAGS_H

/* obey_simple_control_flags — bit indices for obey_individual_simple_control.simple_control_flags
 * (unsigned __int8 @ +5; headers_ref has the struct but no enum). Use as (1u << _bit).
 * Names are setter-anchored in action_obey_command_begin/perform/control.c:
 *   bit 0 direct_movement: set by the move/direction obey atoms together with the
 *         directmovement vector+facing; gates the direct-steering branch everywhere.
 *   bit 2 jump: set by the running-jump and targeted-jump obey atoms.
 *   bit 3 jump_begun: latched by action_obey_control once the jump order is issued;
 *         action_obey_command_perform requires it together with airborne.
 *   bit 4 jump_targeted: set only by the targeted-jump atom (explicit velocity parameters);
 *         action_obey_control reads (flags >> 4) & 1 into orders.move.jump_targeted.
 * Bit 1 is set only via the |= 3 in action_obey_command_begin's halt path — unnamed. */
/* DB-AUTHORITATIVE (was a GUESS before extraction). DB $5F14168C names all 5 bits including bit 1
 * (previously the unnamed |= 3 halt path). Canonical DB names first; the pre-existing _obey_simple_control_*
 * consumer identifiers are kept as same-value aliases. DB confirms bit 3 = jump_jumped (local "jump_begun").
 *
 * CANONICAL HOME for the $5F14168C enumerators. src/headers/obey_simple_flags.h held a second
 * definition of the same six names (merged provenance: "types_enum_values
 * _5F14168CA5C13897CF031FE97B7BC729 carries these named values verbatim") and was deleted
 * 2026-08-07 — two enum bodies for one DB enum is an ODR redefinition that makes clang leave the
 * dependent record incomplete in header_layout's bulk probe. */
enum obey_simple_control_flags {
	_obey_simple_directmovement_bit = 0x0,
	_obey_simple_control_direct_movement_bit = 0x0,                 /* legacy consumer alias */
	_obey_simple_directmovement_update_continuously_bit = 0x1,
	_obey_simple_jump_bit = 0x2,
	_obey_simple_control_jump_bit = 0x2,                            /* legacy consumer alias */
	_obey_simple_jump_jumped_bit = 0x3,
	_obey_simple_control_jump_begun_bit = 0x3,                      /* legacy consumer alias (guessed name) */
	_obey_simple_jump_targeted_bit = 0x4,
	_obey_simple_control_jump_targeted_bit = 0x4,                   /* legacy consumer alias */
	NUMBER_OF_OBEY_SIMPLE_FLAGS = 0x5,
};

#endif /* OBEY_SIMPLE_CONTROL_FLAGS_H */
