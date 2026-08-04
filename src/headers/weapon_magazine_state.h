/* Bound to DB enum types_enum_values _92862746A34F0D0F5438469B143493B2 (2026-07-20 BLOCKED-retry). */
#ifndef WEAPON_MAGAZINE_STATE_H
#define WEAPON_MAGAZINE_STATE_H

/* weapon_magazine_state — runtime reload phase of one weapon magazine (weapon_magazine.state).
 *
 * DB-AUTHORITATIVE (was a GUESS before extraction). The IDA DB now carries the compiled enum
 * _92862746A34F0D0F5438469B143493B2. Canonical names below; the pre-existing consumer identifiers
 * (weapon_magazine_* reload state machine, 12 sites) are retained as same-value aliases. Prior guess
 * corrected: value 2 was guessed "reloaded" (DB _magazine_unchambered). */
enum weapon_magazine_state {
	_magazine_idle = 0,
	_weapon_magazine_idle = 0,            /* legacy consumer alias */
	_magazine_reloading = 1,
	_weapon_magazine_reloading = 1,       /* legacy consumer alias */
	_magazine_unchambered = 2,
	_weapon_magazine_reloaded = 2,        /* legacy consumer alias (guessed name) */
	_magazine_chambering = 3,
	_weapon_magazine_chambering = 3,      /* legacy consumer alias */
	NUMBER_OF_WEAPON_MAGAZINE_STATES = 4,
};

#endif /* WEAPON_MAGAZINE_STATE_H */
