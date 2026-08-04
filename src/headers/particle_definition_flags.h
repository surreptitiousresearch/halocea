#ifndef PARTICLE_DEFINITION_FLAGS_H
#define PARTICLE_DEFINITION_FLAGS_H

/* particle_definition_flags — bit indices for particle_definition.flags.
 * DB-verified: types_enum_values _8D49E739CCB226B4F0FDF8916D1A67F5 carries these named
 * bit indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum particle_definition_flags {
	_particle_can_animate_backwards_bit = 0x0,
	_particle_animation_stops_at_rest_bit = 0x1,
	_particle_animation_starts_on_random_frame_bit = 0x2,
	_particle_animates_at_frame_time_bit = 0x3,
	_particle_dies_at_rest_bit = 0x4,
	_particle_dies_on_contact_with_structure_bit = 0x5,
	_particle_tint_with_diffuse_texture_bit = 0x6,
	_particle_dies_on_contact_with_water_bit = 0x7,
	_particle_dies_on_contact_with_air_bit = 0x8,
	_particle_self_illuminated_bit = 0x9,
	_particle_random_u_mirror_bit = 0xA,
	_particle_random_v_mirror_bit = 0xB,
	NUMBER_OF_PARTICLE_DEFINITION_FLAGS = 0xC,
};

#endif /* PARTICLE_DEFINITION_FLAGS_H */

// particle_definition_flags: DB-verified via types_enum_values _8D49E739CCB226B4F0FDF8916D1A67F5 (anonymous compiled enum, names sans leading _)
