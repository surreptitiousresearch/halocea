/* Bound to DB enum types_enum_values _5252EF4364456B50D1CBA728D8ACA19B (2026-07-20 BLOCKED-retry). */
#ifndef UNIT_ANIMATION_OVERLAY_ACTION_H
#define UNIT_ANIMATION_OVERLAY_ACTION_H

/* unit_animation_overlay_action — values of unit_animation.overlay_action and the `overlay_action`
 * argument of unit_animation_start_overlay_action.
 *
 * DB-AUTHORITATIVE (was a GUESS before extraction). The IDA DB now carries the compiled enum
 * _5252EF4364456B50D1CBA728D8ACA19B (8 values, NUMBER=8). Canonical DB names below; the reconstructed
 * consumer identifiers (fire_1/2, charged_1/2, chamber_1/2) are kept as same-value aliases. The prior
 * reconstruction was missing value 7 (_unit_overlay_action_user_animation) and had NUMBER=7; corrected. */
enum unit_animation_overlay_action {
	_unit_overlay_action_none = 0,
	_unit_animation_overlay_action_none = 0,        /* legacy consumer alias */
	_unit_overlay_action_weapon_fire_primary = 1,
	_unit_animation_overlay_action_fire_1 = 1,      /* legacy consumer alias */
	_unit_overlay_action_weapon_fire_secondary = 2,
	_unit_animation_overlay_action_fire_2 = 2,      /* legacy consumer alias */
	_unit_overlay_action_weapon_primary_charged = 3,
	_unit_animation_overlay_action_charged_1 = 3,   /* legacy consumer alias */
	_unit_overlay_action_weapon_secondary_charged = 4,
	_unit_animation_overlay_action_charged_2 = 4,   /* legacy consumer alias */
	_unit_overlay_action_weapon_primary_chamber = 5,
	_unit_animation_overlay_action_chamber_1 = 5,   /* legacy consumer alias */
	_unit_overlay_action_weapon_secondary_chamber = 6,
	_unit_animation_overlay_action_chamber_2 = 6,   /* legacy consumer alias */
	_unit_overlay_action_user_animation = 7,
	NUMBER_OF_UNIT_OVERLAY_ACTIONS = 8,
	NUMBER_OF_UNIT_ANIMATION_OVERLAY_ACTIONS = 8,   /* legacy consumer alias */
};

#endif /* UNIT_ANIMATION_OVERLAY_ACTION_H */
