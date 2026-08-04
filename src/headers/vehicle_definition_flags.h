#ifndef VEHICLE_DEFINITION_FLAGS_H
#define VEHICLE_DEFINITION_FLAGS_H

/* vehicle_definition_flags — bit indices for vehicle_definition.flags (via definition->vehicle.flags).
 * Source: reference enum _A4817957D351269BB8A1CA942071DAE2 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum vehicle_definition_flags {
	_vehicle_speed_wakes_physics_bit = 0x0,
	_vehicle_turn_wakes_physics_bit = 0x1,
	_vehicle_driver_power_wakes_physics_bit = 0x2,
	_vehicle_gunner_power_wakes_physics_bit = 0x3,
	_vehicle_control_opposite_speed_sets_jump_bit = 0x4,
	_vehicle_slide_wakes_physics_bit = 0x5,
	_vehicle_kills_riders_at_terminal_velocity_bit = 0x6,
	_vehicle_causes_collision_damage_bit = 0x7,
	_vehicle_ai_weapon_cannot_rotate_bit = 0x8,
	_vehicle_ai_does_not_require_driver_bit = 0x9,
	_vehicle_ai_unused_bit = 0xA,
	_vehicle_ai_driver_enable_bit = 0xB,
	_vehicle_ai_driver_flying_bit = 0xC,
	_vehicle_ai_driver_nondirectional_bit = 0xD,
	_vehicle_ai_driver_hovering_bit = 0xE,
	NUMBER_OF_VEHICLE_DEFINITION_FLAGS = 0xF,
};

#endif /* VEHICLE_DEFINITION_FLAGS_H */

// vehicle_definition_flags: DB-verified via types_enum_values _A4817957D351269BB8A1CA942071DAE2 (anonymous compiled enum, names sans leading _)
