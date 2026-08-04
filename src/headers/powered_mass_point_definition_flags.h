#ifndef POWERED_MASS_POINT_DEFINITION_FLAGS_H
#define POWERED_MASS_POINT_DEFINITION_FLAGS_H

/* powered_mass_point_definition_flags — bit indices for powered_mass_point_definition.flags
 * (physics tag; pmp_def->flags in physics_*.c). Verified against physics_update_old.c: bit0 gates
 * ground_friction_velocity, bit1 water_friction_velocity, bit2 air_friction_velocity,
 * bit3 water_lift_ratio, bit4 air_lift_ratio, bit5 thrust, bit6 antigrav_height.
 * Source: reference enum _ACE77A7A44906E48E96C2E9246FBAC9F (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum powered_mass_point_definition_flags {
	_powered_mass_point_ground_friction_bit = 0x0,
	_powered_mass_point_water_friction_bit = 0x1,
	_powered_mass_point_air_friction_bit = 0x2,
	_powered_mass_point_water_lift_bit = 0x3,
	_powered_mass_point_air_lift_bit = 0x4,
	_powered_mass_point_thrust_bit = 0x5,
	_powered_mass_point_antigrav_bit = 0x6,
	NUMBER_OF_POWERED_MASS_POINT_DEFINITION_FLAGS = 0x7,
};

#endif /* POWERED_MASS_POINT_DEFINITION_FLAGS_H */

// powered_mass_point_definition_flags: DB-verified via types_enum_values _ACE77A7A44906E48E96C2E9246FBAC9F (anonymous compiled enum, names sans leading _)
