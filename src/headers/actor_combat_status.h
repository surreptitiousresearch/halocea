#ifndef ACTOR_COMBAT_STATUS_H
#define ACTOR_COMBAT_STATUS_H

/* actor_combat_status — an actor's AI awareness/combat level, a monotonic 0..7 ladder. Shared by all
 * four fields that spell it: state.combat_status (the resolved level), state.artificial_combat_status
 * (scripted alertness floor), state.suspicion_combat_status and stimuli.suspicion_combat_status
 * (perception-driven running level).
 *
 * NOT in headers_ref/DB as an enum (plain __int16) and no name-string table survives. Reconstructed by
 * fresh reasoning; the ladder is the classic Halo actor awareness enum and is CORROBORATED by this
 * binary at several independent points (so the labels are a low-risk inference, not a blind guess):
 *   - actor_situation_combat_status_update: status >= 7 sets state.had_visible_enemy  => 7 = visible (PROVEN).
 *     status >= 4 drives certain_combat_timer, status < 4 drives uncertain_combat_timer => the
 *     uncertain/certain split sits at 4/5. status > 0 gates in_combat_timer => 0 = not in combat.
 *   - actor_stimulus_weapon_detonation sets suspicion = 2 when it merely HEARS an explosion => 2 = alert.
 *   - actor_stimulus_enter_combat_perceived_enemy sets suspicion = 6 on perceiving an enemy => 6 = definite.
 *   - action_*_control gate aggressive behaviour (fight / charge) on combat_status >= 5 => 5 = certain enemy.
 *   - actor_action_set_default_state seeds combat_status = 2 (alert) as the scripted resting level.
 *
 * HISTORIC (names since DB-verified): the exact WORD for each rung (asleep/idle/alert/active/uncertain/certain/
 * definite/visible) is the documented Halo convention. The rung POSITIONS 0, 2, 4/5, 6, 7 are anchored
 * by the binary above; 1 (idle) and 3 (active) are pure convention fill between anchored rungs. */
enum actor_combat_status {
	_actor_combat_status_none = 0,      /* PROVEN: not-in-combat (status>0 gates in_combat_timer) */
	_actor_combat_status_wary = 1,        /* (name now DB-authoritative; historic guess note) GUESS: convention fill */
	_actor_combat_status_investigate = 2,       /* corroborated: heard-detonation stimulus */
	_actor_combat_status_definite = 3,      /* (name now DB-authoritative; historic guess note) GUESS: convention fill (below the <3 uncover boundary) */
	_actor_combat_status_certain = 4,   /* corroborated: uncertain/certain timer split at 4 */
	_actor_combat_status_clear_los = 5,     /* corroborated: fight/charge enabled at >=5 */
	_actor_combat_status_dangerous = 6,    /* corroborated: perceived-enemy stimulus */
	_actor_combat_status_visible = 7,     /* PROVEN: had_visible_enemy set at >=7 */
	NUMBER_OF_ACTOR_COMBAT_STATUS_LEVELS = 8,
};

#endif /* ACTOR_COMBAT_STATUS_H */

// actor_combat_status: DB-verified via types_enum_values _3A53CA2E3A2554131BD610CECB52AAEA (names now authoritative, superseding the earlier reconstruction; magic2 sweep 2026-07-18)
