/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#ifndef UNIT_GRENADE_THROW_STATE_H
#define UNIT_GRENADE_THROW_STATE_H

/* unit_grenade_throw_state — values of _unit_datum.grenade_throw_state (char @ +153/0x99).
 *
 * NOT from headers_ref: no enum for this member exists there or in the IDA DB. Names are derived from
 * the state machine's own DB-named driver functions (each value is written by exactly one stage):
 *   1 <- unit_throw_grenade_begin @ throw start
 *   2 <- unit_throw_grenade_move_to_hand (grenade primed and in hand)
 *   3 <- unit_throw_grenade_release (grenade has left the hand); unit_died / unit_update reset to 0.
 * Semantics of "!= 3": a throw is underway and the live grenade is still held (see
 * any_unit_is_dangerous, should_draw_object). */
enum unit_grenade_throw_state {
	_grenade_throw_state_none = 0,
	_grenade_throw_state_begin = 1,
	_grenade_throw_state_in_hand = 2,
	_grenade_throw_state_released = 3,
	NUMBER_OF_GRENADE_THROW_STATES = 4,
};

#endif /* UNIT_GRENADE_THROW_STATE_H */
