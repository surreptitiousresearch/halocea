#ifndef UNIT_DESIRED_ANIMATION_STATE_H
#define UNIT_DESIRED_ANIMATION_STATE_H

/* unit_desired_animation_state — the vocabulary of unit_control_data.animation_state /
 * unit.animation.desired_state (NOT the unit_animation_state enum; a separate, smaller set).
 * NOT in headers_ref/DB as an enum. Names recovered from two independent in-binary anchors:
 *   1. global_movement_animation_states @0x82114DB8 (const __int16[5], DB-typed) maps
 *      actor_movement_type -> this value: alert->1, asleep->0, combat->3, panic->5, flaming->6
 *      (actor_unit_control.c: control.animation_state = table[actor->output.movement_type]).
 *   2. unit_update_animation.c maps this value -> base seat label:
 *      0->asleep, 1/2->alert, 3->stand-or-crouch, 4->stand, 5->flee, default(6+)->flaming.
 * Both anchors agree on 0/1/3/5/6. Values 2 and 4 are never written anywhere in src/ —
 * named from their base-seat mapping only (GUESS). */
enum unit_desired_animation_state {
	_unit_animation_state_asleep = 0,
	_unit_animation_state_alert = 1,
	_unit_animation_state_suspicious = 2,      /* (name now DB-authoritative; historic guess note) GUESS: alert base seat, never written */
	_unit_animation_state_in_combat = 3,      /* stand (or crouch when control crouching) */
	_unit_animation_state_wary = 4,       /* (name now DB-authoritative; historic guess note) GUESS: stand base seat, never written */
	_unit_animation_state_flee = 5,       /* flee base seat */
	_unit_animation_state_flaming = 6,
	NUMBER_OF_UNIT_ANIMATION_STATES = 7,
};

#endif /* UNIT_DESIRED_ANIMATION_STATE_H */

// unit_animation_state (desired/base-seat): DB-verified via types_enum_values _5D851471DC20289CC8EC295C9C6E643D (names now authoritative, superseding the earlier reconstruction; magic2 sweep 2026-07-18)
