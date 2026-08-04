#ifndef PROP_PERCEPTION_STATE_H
#define PROP_PERCEPTION_STATE_H

/* prop_perception_state — how far an actor's perception of one prop (its per-unit knowledge record,
 * prop_datum.state) has progressed. A 0..5 ladder from "unknown" up through active perception and then
 * the two "orphan" (remembered-but-unit-gone) states.
 *
 * NOT in headers_ref/DB as an enum (plain __int16); no name-string table. Reconstructed by fresh
 * reasoning over the setter functions (their names anchor 4 of the 6 rungs) and the perception update:
 *   0 none      — prop_status_refresh clears the record to 0 (no current knowledge).            [setter-anchored]
 *   2 alert     — actor_perception_update treats state >= 2 as "in use / actively noticed" (the
 *                 perceived-enough threshold). [boundary-anchored]
 *   3 perceived — actor_perception_become_acknowledged and prop_get_base_by_unit_index set 3 when the
 *                 prop becomes an acknowledged/perceived enemy.                                  [setter-anchored]
 *   4 orphan    — prop_setup_orphan sets 4 (the prop's unit is gone but the actor still remembers it). [setter-anchored]
 *   5 orphan_inspected — actor_perception_abandoned_search sets 5 (an orphan that has since been
 *                 investigated/searched).                                                        [setter-anchored]
 *
 * HISTORIC (names since DB-verified): rung 1 = acquiring is convention fill between "none" (0) and "alert" (2) — no
 * setter writes 1 directly (the update computes it), so its label is inferred, not proven. The word
 * choices for 2/3/4/5 follow the meaning of their setter functions; the positions are setter-anchored.
 * (An older inline comment in actor_perception_update called state 0 "acquiring"; that conflicts with
 * prop_status_refresh clearing to 0 and is superseded by this fresh reading.) */
enum prop_perception_state {
	_prop_state_unacknowledged = 0,
	_prop_state_becoming_acknowledged = 1,          /* (name now DB-authoritative; historic guess note) GUESS: convention fill; never written directly */
	_prop_state_becoming_unacknowledged = 2,
	_prop_state_acknowledged = 3,
	_prop_state_uninspected_orphan = 4,
	_prop_state_inspected_orphan = 5,
	NUMBER_OF_PROP_STATES = 6,
};

#endif /* PROP_PERCEPTION_STATE_H */

// prop_state: DB-verified via types_enum_values _48889CF285DAFB4D595CBC163A8AE260 (names now authoritative, superseding the earlier reconstruction; magic2 sweep 2026-07-18)
