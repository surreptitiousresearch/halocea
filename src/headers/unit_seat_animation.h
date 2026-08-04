#ifndef UNIT_SEAT_ANIMATION_H
#define UNIT_SEAT_ANIMATION_H

/* unit_seat_animation — index into an animation_graph_unit_seat's own animations tag_block
 * (the per-seat overlay/additive animation table).
 * Source: DB compiled enum types_enum_values _D290F0FD4D19DD165D896A7FA993EA14. Ground truth. */
enum unit_seat_animation {
	_unit_seat_animation_airborne_dead = 0,
	_unit_seat_animation_landing_dead = 1,
	_unit_seat_animation_acceleration_front_back = 2,
	_unit_seat_animation_acceleration_left_right = 3,
	_unit_seat_animation_acceleration_up_down = 4,
	_unit_seat_animation_push_impact = 5,
	_unit_seat_animation_twist_impact = 6,
	_unit_seat_animation_seat_enter = 7,
	_unit_seat_animation_seat_exit = 8,
	_unit_seat_animation_looking = 9,
	_unit_seat_animation_mouth_aperture = 10,
	_unit_seat_animation_emotions = 11,
	_unit_seat_animation_unused3 = 12,
	_unit_seat_animation_user0 = 13,
	_unit_seat_animation_user1 = 14,
	_unit_seat_animation_user2 = 15,
	_unit_seat_animation_user3 = 16,
	_unit_seat_animation_user4 = 17,
	_unit_seat_animation_user5 = 18,
	_unit_seat_animation_user6 = 19,
	_unit_seat_animation_user7 = 20,
	_unit_seat_animation_user8 = 21,
	_unit_seat_animation_user9 = 22,
	_unit_seat_animation_flying_front = 23,
	_unit_seat_animation_flying_back = 24,
	_unit_seat_animation_flying_left = 25,
	_unit_seat_animation_flying_right = 26,
	_unit_seat_animation_opening = 27,
	_unit_seat_animation_closing = 28,
	_unit_seat_animation_hovering = 29,
	NUMBER_OF_UNIT_SEAT_ANIMATIONS = 30,
};

#endif /* UNIT_SEAT_ANIMATION_H */
