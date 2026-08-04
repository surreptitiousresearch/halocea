/* Bound to DB enum types_enum_values _5B04D6C4DFB5812822CA1995E3F483DC (2026-07-20 BLOCKED-retry). */
#ifndef WEAPON_TRIGGER_STATE_H
#define WEAPON_TRIGGER_STATE_H

/* weapon_trigger_state — runtime firing phase of one weapon trigger (weapon_trigger.state, and the
 * `firing_state` local decoded from it in weapon_update).
 *
 * DB-AUTHORITATIVE (was a GUESS before extraction). The IDA DB now carries the compiled enum
 * _5B04D6C4DFB5812822CA1995E3F483DC. Canonical names below; the pre-existing consumer identifiers
 * (weapon_update / weapon_reset, 14 sites) are retained as same-value aliases to avoid a cross-file
 * rename. Prior guesses corrected: 1 was guessed "firing" (DB overloading), 6 "releasing" (DB spewing),
 * 7 "firing_held" (DB locked), 8 "ready" (DB uninitialized). */
enum weapon_trigger_state {
	_trigger_idle = 0,
	_weapon_trigger_idle = 0,             /* legacy consumer alias */
	_trigger_overloading = 1,
	_weapon_trigger_firing = 1,           /* legacy consumer alias (guessed name) */
	_trigger_charging = 2,
	_weapon_trigger_charging = 2,         /* legacy consumer alias */
	_trigger_charged = 3,
	_weapon_trigger_charged = 3,          /* legacy consumer alias */
	_trigger_recovering = 4,
	_weapon_trigger_recovering = 4,       /* legacy consumer alias */
	_trigger_tracking = 5,
	_weapon_trigger_tracking = 5,         /* legacy consumer alias */
	_trigger_spewing = 6,
	_weapon_trigger_releasing = 6,        /* legacy consumer alias (guessed name) */
	_trigger_locked = 7,
	_weapon_trigger_firing_held = 7,      /* legacy consumer alias (guessed name) */
	_trigger_uninitialized = 8,
	_weapon_trigger_ready = 8,            /* legacy consumer alias (guessed name) */
	NUMBER_OF_TRIGGER_STATES = 9,
	NUMBER_OF_WEAPON_TRIGGER_STATES = 9,  /* legacy consumer alias */
};

#endif /* WEAPON_TRIGGER_STATE_H */
