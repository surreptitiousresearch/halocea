#pragma once

/* unit_grenade_throw_state — values of _unit_datum.grenade_throw_state (char @ +153/0x99).
 *
 * CORRECTED 2026-08-13 (enum drain round 2): this header previously claimed "no enum for
 * this member exists in the IDA DB" and carried invented names — the DB names all four
 * values (types_enum_values $16B6CFD28B61E4BAE2C4614C85184D4F, verbatim below). The prior
 * reconstruction's state-machine reading survives and corroborates the DB names exactly
 * (each value is written by one DB-named driver function):
 *   1 (_wind_up) <- unit_throw_grenade_begin @ throw start
 *   2 (_in_hand) <- unit_throw_grenade_move_to_hand (grenade primed and in hand)
 *   3 (_ending)  <- unit_throw_grenade_release (grenade has left the hand);
 *   0 (_idle)    <- unit_died / unit_update reset.
 * Semantics of "!= _unit_grenade_throw_ending": a throw is underway and the live grenade is
 * still held (see any_unit_is_dangerous, should_draw_object). */
enum unit_grenade_throw_state {
	_unit_grenade_throw_idle = 0,
	_unit_grenade_throw_wind_up = 1,
	_unit_grenade_throw_in_hand = 2,
	_unit_grenade_throw_ending = 3,
	NUMBER_OF_UNIT_GRENADE_ATTACK_STATES = 4,
};

// unit_grenade_throw_state: DB-verified via types_enum_values $16B6CFD28B61E4BAE2C4614C85184D4F
