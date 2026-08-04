/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#ifndef SWARM_COMPONENT_FLAGS_H
#define SWARM_COMPONENT_FLAGS_H

/* swarm_component_flags — bit indices for swarm_component_datum.flags (__int16 @ +2). Use as
 * (1u << _bit). GUESS (marked): no enum in headers_ref or the IDA DB; names are recovered from the
 * two consumers, infection_swarm_control.c and infection_swarm_aim_jump.c:
 *   bit 0 melee_engaged: set when the combat target is in melee range and the attack delay expired;
 *         drives melee_attack_state=3 and gates the ballistic leap in aim_jump (&1 && prop != -1).
 *   bit 1 attached: set while the member biped is parented to a host unit; cleared on detach and
 *         while airborne-unparented.
 *   bit 2 wander_valid: set once the wander work-area (timers/vector/angle) has been initialised.
 *   bit 3 obey_direct: obey/direct-movement steering active (selects movement mode 6).
 *   bit 4 jump_pending: latched when the obey lunge fires (with simple_control jump state); read by
 *         the jump/lunge trigger and by aim_jump, which consumes and clears it. */
enum swarm_component_flags {
	_swarm_component_melee_engaged_bit = 0x0,
	_swarm_component_attached_bit = 0x1,
	_swarm_component_wander_valid_bit = 0x2,
	_swarm_component_obey_direct_bit = 0x3,
	_swarm_component_jump_pending_bit = 0x4,
};

#endif /* SWARM_COMPONENT_FLAGS_H */

/* DB-verified via types_enum_values _56A1620364F091D0F775FB9EFA5597FD. The DB carries this
 * same enum with canonical names (bit index in parens); the local identifiers above are the
 * pre-existing consumer names and are kept to avoid a cross-file rename:
 *   _swarm_component_attacking_in_melee_bit (0) == _swarm_component_melee_engaged_bit
 *   _swarm_component_attached_to_unit_bit    (1) == _swarm_component_attached_bit
 *   _swarm_component_wander_bit              (2) == _swarm_component_wander_valid_bit
 *   _swarm_component_obey_bit                (3) == _swarm_component_obey_direct_bit
 *   _swarm_component_obey_desire_jump_bit    (4) == _swarm_component_jump_pending_bit
 *   NUMBER_OF_SWARM_COMPONENT_FLAGS = 5 */
