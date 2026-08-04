#ifndef UNIT_SEAT_FLAGS_H
#define UNIT_SEAT_FLAGS_H

/* unit_seat_flags — bit indices for unit_seat.flags (unit definition seat block; seat->flags).
 * Source: reference enum _D4C24A27776BC65FA6265C11E741424A (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum unit_seat_flags {
	_unit_seat_is_invisible_bit = 0x0,
	_unit_seat_is_locked_bit = 0x1,
	_unit_seat_is_driver_bit = 0x2,
	_unit_seat_is_gunner_bit = 0x3,
	_unit_seat_has_third_person_camera_bit = 0x4,
	_unit_seat_allows_weapons_bit = 0x5,
	_unit_seat_third_person_on_enter_bit = 0x6,
	_unit_seat_slave_first_person_camera_bit = 0x7,
	_unit_seat_allow_vehicle_communication_animations_bit = 0x8,
	_unit_seat_not_valid_without_driver_bit = 0x9,
	_unit_seat_allow_noncombatants_bit = 0xA,
	NUMBER_OF_UNIT_SEAT_FLAGS = 0xB,
};

#endif /* UNIT_SEAT_FLAGS_H */

// unit_seat_flags: DB-verified via types_enum_values _D4C24A27776BC65FA6265C11E741424A (anonymous compiled enum, names sans leading _)
