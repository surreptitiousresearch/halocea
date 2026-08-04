/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _064FEB99C87B33E28B0DC98615EB02A2 (DB names use the _vehicle_function_* prefix; the _vehicle_export_*
 * identifiers below are the consumer-facing names). Note value 13: DB names it _vehicle_function_unused
 * (the local "_vehicle_export_walk" label is a reconstruction and has no consumer). */
#ifndef VEHICLE_EXPORT_FUNCTION_MODE_H
#define VEHICLE_EXPORT_FUNCTION_MODE_H

/* vehicle_export_function_mode — the vehicle tag's per-slot export-function source selector
 * (_vehicle_definition.function_modes[4], the "A in".."D in" tag fields).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source in this binary (tag-editor option
 * strings stripped; no enum in headers_ref/DB) — rationale: names are the published Halo CE
 * tag-definition option list, and vehicle_export_function_values @0x83760620 matches it per case:
 * 1 |throttle|/range (speed absolute), 2/3 forward/reverse throttle, 4-6 slide ratios, 7 max of
 * speed+slide, 8-10 turn ratios, 11/12 vehicle flag bits 2/3 (crouch/jump), 14-16 |velocity| gated by
 * air/water/ground object flags, 17 velocity dot forward, 18/19 velocity dot up, 20/21
 * left/right tread over wheel circumference, 22/23 |throttle -/+ turn| (tread velocities), 24-27 wheel
 * position (tire positions), 28-31 grouped with speed absolute (tire velocities), 32 perpendicular
 * velocity squared (wingtip contrail), 33 hover, 34 thrust, 35 the airborne-gear throttle blend
 * ("engine hack"), 36 thrust-scaled speed (wingtip contrail new). 13 ("walk") is unhandled (0). */
enum vehicle_export_function_mode {
	_vehicle_export_none = 0,
	_vehicle_export_speed_absolute = 1,
	_vehicle_export_speed_forward = 2,
	_vehicle_export_speed_backward = 3,
	_vehicle_export_slide_absolute = 4,
	_vehicle_export_slide_left = 5,
	_vehicle_export_slide_right = 6,
	_vehicle_export_speed_slide_maximum = 7,
	_vehicle_export_turn_absolute = 8,
	_vehicle_export_turn_left = 9,
	_vehicle_export_turn_right = 10,
	_vehicle_export_crouch = 11,
	_vehicle_export_jump = 12,
	_vehicle_export_walk = 13,
	_vehicle_export_velocity_air = 14,
	_vehicle_export_velocity_water = 15,
	_vehicle_export_velocity_ground = 16,
	_vehicle_export_velocity_forward = 17,
	_vehicle_export_velocity_left = 18,
	_vehicle_export_velocity_up = 19,
	_vehicle_export_left_tread_position = 20,
	_vehicle_export_right_tread_position = 21,
	_vehicle_export_left_tread_velocity = 22,
	_vehicle_export_right_tread_velocity = 23,
	_vehicle_export_front_left_tire_position = 24,
	_vehicle_export_front_right_tire_position = 25,
	_vehicle_export_back_left_tire_position = 26,
	_vehicle_export_back_right_tire_position = 27,
	_vehicle_export_front_left_tire_velocity = 28,
	_vehicle_export_front_right_tire_velocity = 29,
	_vehicle_export_back_left_tire_velocity = 30,
	_vehicle_export_back_right_tire_velocity = 31,
	_vehicle_export_wingtip_contrail = 32,
	_vehicle_export_hover = 33,
	_vehicle_export_thrust = 34,
	_vehicle_export_engine_hack = 35,
	_vehicle_export_wingtip_contrail_new = 36,
	NUMBER_OF_VEHICLE_EXPORT_FUNCTION_MODES = 37,
};

#endif /* VEHICLE_EXPORT_FUNCTION_MODE_H */
