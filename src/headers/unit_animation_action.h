/* Reconstruction (no DB/PDB type) — STILL-ABSENT after extraction (re-checked 2026-07-20 BLOCKED-retry).
 * DO NOT bind to DB enum _80A77488063061A73F6B1F06BE8238A2 (_unit_action_*): that is a DISTINCT sibling
 * enum. It orders overheat=8, throw_grenade=9, but the unit_animation.action switch in this binary
 * (unit_animation_start_action @0x836C8340) maps action 8 -> weapon-class throw_grenade animation
 * (li r4,0x14 @0x836C8470) and action 9 -> weapon-type overheat (li r5,9 @0x836C8478) — the opposite
 * 8/9 ordering. No DB enum matches the animation-action field's ordering; kept as a reconstruction. */
#ifndef UNIT_ANIMATION_ACTION_H
#define UNIT_ANIMATION_ACTION_H

/* unit_animation_action — values of unit_animation.action and the `action` argument of
 * unit_animation_start_action.
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source for the action NAMES —
 * rationale: unit_animation_start_action @0x836C8340 maps each action 1..9 to exactly one entry of the
 * binary's own weapon-class/weapon-type animation label tables (which ARE ground truth, see
 * weapon_class_animation.h / weapon_type_animation.h): 1->disarm, 2->drop, 3->ready, 4->put-away,
 * 5->reload-1, 6->reload-2, 7->melee, 8->throw-grenade, 9->overheat. Each action is named after its
 * bound animation label. Cross-check: unit_handle_weapon_state_change routes weapon_datum_state
 * primary/secondary reload to actions 5/6, matching reload-1/reload-2. */
enum unit_animation_action {
	_unit_animation_action_none = 0,
	_unit_animation_action_disarm = 1,        /* weapon-class "disarm" (21) */
	_unit_animation_action_drop = 2,          /* weapon-class "drop" (22) */
	_unit_animation_action_ready = 3,         /* weapon-class "ready" (23) */
	_unit_animation_action_put_away = 4,      /* weapon-class "put-away" (24) */
	_unit_animation_action_reload_1 = 5,      /* weapon-type "reload-1" (0) */
	_unit_animation_action_reload_2 = 6,      /* weapon-type "reload-2" (1) */
	_unit_animation_action_melee = 7,         /* weapon-type "melee" (8) */
	_unit_animation_action_throw_grenade = 8, /* weapon-class "throw-grenade" (20) */
	_unit_animation_action_overheat = 9,      /* weapon-type "overheat" (9) */
	NUMBER_OF_UNIT_ANIMATION_ACTIONS = 10,
};

#endif /* UNIT_ANIMATION_ACTION_H */
