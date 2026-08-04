#ifndef UNIT_ANIMATION_STATE_H
#define UNIT_ANIMATION_STATE_H

/* unit_animation_state — values of unit_animation.state (char @ +11; _unit_datum.unit.animation.state).
 *
 * NOT from headers_ref: no enum for this member survives there or in the IDA DB (the member is a plain
 * char everywhere). Names are recovered from the binary itself:
 *   - unit_animation_set_state @0x836CE230 maps each state to an index into one of the two animation
 *     label tables in .data: the weapon-class label table @0x84185B80 ("idle", "gesture", ...,
 *     "throw-grenade"[20], ..., "leap-melee"[50], "zapping"[51]) and the unit-seat label table
 *     @0x84185D90 ("airborne-dead"[0], "landing-dead"[1], ..., "look"[9], ..., "opening"[27],
 *     "closing"[28], "hovering"[29]). Cross-check: the same function reads seat label [9] for the
 *     "looking" overlay, which is exactly "look" in the table.
 *   - States with no label binding in that switch are named from their DB-named driver functions:
 *     23 unit_ping_animation (hard ping / stagger), 26 unit_enter_seat, 27 unit_exit_seat_end /
 *     vehicle unload, 28 unit_start_user_animation ("custom animation"), 29
 *     unit_start_animation_impulse, 42 unit_shield_sapping_update (weapon-class label 51 "zapping").
 * State-machine classes per unit_animation_state_interruptable @0x836C7EF8: 24/25 are the two death
 * states; 29-35/39/41 are committed actions that yield only to a hard ping (23). */
enum unit_animation_state {
	_unit_state_idle = 0,             /* weapon label 0  "idle" */
	_unit_state_gesture = 1,          /* weapon label 1  "gesture" */
	_unit_state_turn_left = 2,        /* weapon label 2  "turn-left" */
	_unit_state_turn_right = 3,       /* weapon label 3  "turn-right" */
	_unit_state_move_front = 4,       /* weapon label 8  "move-front" */
	_unit_state_move_back = 5,        /* weapon label 9  "move-back" */
	_unit_state_move_left = 6,        /* weapon label 10 "move-left" */
	_unit_state_move_right = 7,       /* weapon label 11 "move-right" */
	_unit_state_stunned_move_front = 8,    /* weapon label 35 "stunned-front" */
	_unit_state_stunned_move_back = 9,     /* weapon label 36 "stunned-back" */
	_unit_state_stunned_move_left = 10,    /* weapon label 37 "stunned-left" */
	_unit_state_stunned_move_right = 11,   /* weapon label 38 "stunned-right" */
	_unit_state_slide_front = 12,     /* weapon label 12 "slide-front" */
	_unit_state_slide_back = 13,      /* weapon label 13 "slide-back" */
	_unit_state_slide_left = 14,      /* weapon label 14 "slide-left" */
	_unit_state_slide_right = 15,     /* weapon label 15 "slide-right" */
	_unit_state_flying_front = 16,    /* seat label 23 "flying-front" */
	_unit_state_flying_back = 17,     /* seat label 24 "flying-back" */
	_unit_state_flying_left = 18,     /* seat label 25 "flying-left" */
	_unit_state_flying_right = 19,    /* seat label 26 "flying-right" */
	_unit_state_airborne = 20,        /* weapon label 16 "airborne" */
	_unit_state_land_soft = 21,       /* weapon label 17 "land-soft" */
	_unit_state_land_hard = 22,       /* weapon label 18 "land-hard" */
	_unit_state_hard_ping = 23,       /* driven by unit_ping_animation; committed states yield only to this */
	_unit_state_dying_airborne = 24,   /* seat label 0 "airborne-dead" */
	_unit_state_dying = 25,    /* seat label 1 "landing-dead" */
	_unit_state_entering_seat = 26,      /* set by unit_enter_seat */
	_unit_state_exiting_seat = 27,       /* set by unit_exit_seat / vehicle_scripting_unload */
	_unit_state_user_animation = 28,/* set by unit_start_user_animation */
	_unit_state_ai_impulse = 29,         /* set by unit_start_animation_impulse */
	_unit_state_melee_attack = 30,           /* weapon label 39 "melee" */
	_unit_state_melee_airborne = 31,  /* weapon label 42 "melee-airborne" */
	_unit_state_melee_continuous = 32,/* weapon label 46 "melee-continuous" */
	_unit_state_throw_grenade = 33,   /* weapon label 20 "throw-grenade" */
	_unit_state_resurrect_front = 34, /* weapon label 44 "resurrect-front" */
	_unit_state_resurrect_back = 35,  /* weapon label 45 "resurrect-back" */
	_unit_state_feeding = 36,         /* weapon label 47 "feeding" */
	_unit_state_opening = 37,            /* seat label 27 "opening" (unit_open) */
	_unit_state_closing = 38,           /* seat label 28 "closing" (unit_close) */
	_unit_state_leap_start = 39,      /* weapon label 48 "leap-start" (unit_leap_begin) */
	_unit_state_leap_airborne = 40,   /* weapon label 49 "leap-airborne" */
	_unit_state_leap_melee = 41,      /* weapon label 50 "leap-melee" */
	_unit_state_shield_sapping = 42,         /* weapon label 51 "zapping" (unit_shield_sapping_update) */
	_unit_state_hovering = 43,        /* seat label 29 "hovering" */
	NUMBER_OF_UNIT_STATES = 44,
};

#endif /* UNIT_ANIMATION_STATE_H */

// unit_state: DB-verified via types_enum_values _601BF61929DF351C9674731C57792E25 (names now authoritative, superseding the earlier reconstruction; magic2 sweep 2026-07-18)
