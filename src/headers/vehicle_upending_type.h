#pragma once

/* vehicle_upending_type — value of vehicle_datum.upending_type (+0x05 of the vehicle body; see
 * src/headers/_vehicle_datum.h), the axis a flipped vehicle is currently righting itself along.
 * vehicle_update.c reads it as `righting_axis` inside the `_vehicle_upending_bit` branch and
 * splits it exactly along this domain: values 3/4 (left/right) drive the torque off the
 * up x forward cross product while 1/2 (back/forward) use the forward axis directly, and the
 * sign flips between each pair (2/4 positive, 1/3 negative); the else branch clears the bit and
 * stores 0 = _vehicle_not_upending, as do vehicle_new.c and vehicle_reset.c.
 * Source: types_enum_values $B376A755F569FE1DFEFCCBE37F00924A (DB-verified). */
enum vehicle_upending_type {
	_vehicle_not_upending = 0,
	_vehicle_upending_along_back = 1,
	_vehicle_upending_along_forward = 2,
	_vehicle_upending_along_left = 3,
	_vehicle_upending_along_right = 4,
	NUMBER_OF_VEHICLE_UPENDING_TYPES = 5,
};

// vehicle_upending_type: DB-verified via types_enum_values $B376A755F569FE1DFEFCCBE37F00924A
