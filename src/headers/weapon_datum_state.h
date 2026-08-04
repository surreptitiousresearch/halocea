#ifndef WEAPON_DATUM_STATE_H
#define WEAPON_DATUM_STATE_H

/* weapon_datum_state — a weapon object's animation/state machine state (_weapon_datum.state).
 * Source: reference enum _E2EB8D477615536CA8EB48DF278947A7 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. Named weapon_datum_state here because src/headers/weapon_state.h
 * already holds the unrelated networked weapon_state struct. */
enum weapon_datum_state {
	weapon_state_idle = 0,
	weapon_state_primary_recoil = 1,
	weapon_state_secondary_recoil = 2,
	weapon_state_primary_chamber = 3,
	weapon_state_secondary_chamber = 4,
	weapon_state_primary_reload = 5,
	weapon_state_secondary_reload = 6,
	weapon_state_primary_charged = 7,
	weapon_state_secondary_charged = 8,
	weapon_state_ready = 9,
	weapon_state_put_away = 10,
	number_of_weapon_states = 11
} ;

#endif /* WEAPON_DATUM_STATE_H */

// weapon_datum_state: DB-verified via types_enum_values _E2EB8D477615536CA8EB48DF278947A7 (anonymous compiled enum, names sans leading _)
