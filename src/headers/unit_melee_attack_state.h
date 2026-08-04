/* Bound to DB enum types_enum_values _0AF69743231D497D879A19C3FC126452 (2026-07-20 BLOCKED-retry). */
#ifndef UNIT_MELEE_ATTACK_STATE_H
#define UNIT_MELEE_ATTACK_STATE_H

/* unit_melee_attack_state — values of _unit_datum.unit.melee_attack_state (char @ +0x95).
 *
 * DB-AUTHORITATIVE (was a GUESS before extraction). The IDA DB now carries the compiled enum
 * _0AF69743231D497D879A19C3FC126452. Canonical DB names below; the consumer identifiers are kept as
 * same-value aliases. Prior guesses corrected: 1 was "attacking" (DB _unit_melee_attack_starting),
 * 2 was "unknown2" (DB _unit_melee_attack_dangerous). */
enum unit_melee_attack_state {
	_unit_melee_attack_none = 0,
	_unit_melee_attack_state_none = 0,        /* legacy consumer alias */
	_unit_melee_attack_starting = 1,
	_unit_melee_attack_state_attacking = 1,   /* legacy consumer alias (guessed name) */
	_unit_melee_attack_dangerous = 2,
	_unit_melee_attack_state_unknown2 = 2,    /* legacy consumer alias (guessed name) */
	_unit_melee_attack_impact = 3,
	_unit_melee_attack_state_impact = 3,      /* legacy consumer alias */
	_unit_melee_attack_continuous = 4,
	_unit_melee_attack_state_continuous = 4,  /* legacy consumer alias */
	NUMBER_OF_UNIT_MELEE_ATTACK_STATES = 5,
};

#endif /* UNIT_MELEE_ATTACK_STATE_H */
