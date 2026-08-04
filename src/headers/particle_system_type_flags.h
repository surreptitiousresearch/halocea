#ifndef PARTICLE_SYSTEM_TYPE_FLAGS_H
#define PARTICLE_SYSTEM_TYPE_FLAGS_H

/* particle_system_type_flags - bit indices for the particle_system_type.flags word.
 * DB-verified: types_enum_values _889B1125BCB8EE7148B380E5A75CC6C8 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum particle_system_type_flags {
	_particle_system_type_type_states_loop_bit = 0,
	_particle_system_type_type_states_loop_forward_backward_bit = 1,
	_particle_system_type_particle_states_loop_bit = 2,
	_particle_system_type_particle_states_loop_forward_backward_bit = 3,
	_particle_system_type_dies_in_water_bit = 4,
	_particle_system_type_dies_in_air_bit = 5,
	_particle_system_type_dies_on_ground_bit = 6,
	_particle_system_type_rotational_sprites_animate_sideways_bit = 7,
	_particle_system_type_disabled_bit = 8,
	_particle_system_type_tint_by_effect_color_bit = 9,
	_particle_system_type_initial_count_scales_bit = 10,
	_particle_system_type_minimum_count_scales_bit = 11,
	_particle_system_type_creation_rate_scales_bit = 12,
	_particle_system_type_scale_scales_bit = 13,
	_particle_system_type_animation_rate_scales_bit = 14,
	_particle_system_type_rotation_rate_scales_bit = 15,
	_particle_system_type_nodraw_first_person_bit = 16,
	_particle_system_type_nodraw_third_person_bit = 17,
	NUMBER_OF_PARTICLE_SYSTEM_TYPE_FLAGS = 18,
};

#endif /* PARTICLE_SYSTEM_TYPE_FLAGS_H */

// particle_system_type_flags: DB-verified via types_enum_values _889B1125BCB8EE7148B380E5A75CC6C8
