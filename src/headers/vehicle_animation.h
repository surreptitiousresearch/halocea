#pragma once

/* vehicle_animation — the fixed slot index into a model_animation_graph's vehicle
 * animation list (animation_graph.vehicle_animations[0].animations, a block of int16_t
 * animation indices). vehicle_preprocess_node_orientations @0x8375D388 reads slots 0,1,2,3
 * and 5 and drives each from the matching motion quantity: slot 0 feeds the aiming screen
 * from vehicle.turn (steering), slot 1 from the lateral velocity triple product (roll),
 * slot 2 from vehicle.speed / maximum_forward_speed (throttle), slot 3 from
 * forward . translational_velocity (velocity), slot 5 from vehicle.wheel /
 * wheel_circumference (ground speed) — a 1:1 positional match with the DB names, and the
 * shipped code simply never plays slot 4 (braking) or 6/7 (occupied/unoccupied).
 * Source: types_enum_values $3DA66E41918B78A587E94700A3051BF4 (DB-verified). */
enum vehicle_animation {
	_vehicle_animation_steering = 0,
	_vehicle_animation_roll = 1,
	_vehicle_animation_throttle = 2,
	_vehicle_animation_velocity = 3,
	_vehicle_animation_braking = 4,
	_vehicle_animation_ground_speed = 5,
	_vehicle_animation_occupied = 6,
	_vehicle_animation_unoccupied = 7,
	NUMBER_OF_VEHICLE_ANIMATIONS = 8,
};

// vehicle_animation: DB-verified via types_enum_values $3DA66E41918B78A587E94700A3051BF4
