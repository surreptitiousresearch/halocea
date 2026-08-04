#ifndef UNIT_ANIMATION_IMPULSE_H
#define UNIT_ANIMATION_IMPULSE_H

/* unit_animation_impulse — AI animation-impulse ids (argument of unit_animation_impulse_get_index and
 * actor_move_animation_impulse).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source for the impulse NAMES —
 * rationale: unit_animation_impulse_get_index @0x836C94E8 maps each impulse 0..13 to exactly one entry
 * of the binary's own weapon-class animation label table (ground truth, see weapon_class_animation.h):
 * 0->berserk, 1->signal-move, 2->signal-attack, 3->warn, 4->surprise-front, 5->surprise-back,
 * 6->evade-left, 7->evade-right, 8->dive-front, 9->dive-back, 10->dive-left, 11->dive-right,
 * 12->celebrate, 13->panic. Each impulse is named after its bound animation label. */
/* layout bound to DB enum: types_enum_values _E1348C23B6F79F01EF2735D5453AE34B */
enum unit_animation_impulse {
	_unit_animation_impulse_berserk = 0x0,
	_unit_animation_impulse_signal_move = 0x1,
	_unit_animation_impulse_signal_attack = 0x2,
	_unit_animation_impulse_signal_warn = 0x3,
	_unit_animation_impulse_surprise_front = 0x4,
	_unit_animation_impulse_surprise_back = 0x5,
	_unit_animation_impulse_evade_left = 0x6,
	_unit_animation_impulse_evade_right = 0x7,
	_unit_animation_impulse_dive_front = 0x8,
	_unit_animation_impulse_dive_back = 0x9,
	_unit_animation_impulse_dive_left = 0xA,
	_unit_animation_impulse_dive_right = 0xB,
	_unit_animation_impulse_vehicle_celebrate = 0xC,
	_unit_animation_impulse_vehicle_panic = 0xD,
	NUMBER_OF_UNIT_ANIMATION_IMPULSES = 0xE,
};

#endif /* UNIT_ANIMATION_IMPULSE_H */
