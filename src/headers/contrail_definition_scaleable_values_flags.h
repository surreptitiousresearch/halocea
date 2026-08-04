#ifndef CONTRAIL_DEFINITION_SCALEABLE_VALUES_FLAGS_H
#define CONTRAIL_DEFINITION_SCALEABLE_VALUES_FLAGS_H

/* contrail_definition_scaleable_values_flags — contrail_definition_scaleable_values flags.
 * DB-verified: types_enum_values _1A0CE18CE7DD4E370B6C0397B3A87332 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum contrail_definition_scaleable_values_flags {
	_contrail_point_generation_rate_bit = 0,
	_contrail_point_velocity_bit = 1,
	_contrail_point_velocity_delta_bit = 2,
	_contrail_point_velocity_cone_angle_bit = 3,
	_contrail_point_velocity_fraction_bit = 4,
	_contrail_animation_rate_scales_bit = 5,
	_contrail_texture_repeats_u_bit = 6,
	_contrail_texture_repeats_v_bit = 7,
	_contrail_texture_animation_u_bit = 8,
	_contrail_texture_animation_v_bit = 9,
	NUMBER_OF_CONTRAIL_DEFINITION_SCALEABLE_VALUES = 10,
};

#endif /* CONTRAIL_DEFINITION_SCALEABLE_VALUES_FLAGS_H */

// contrail_definition_scaleable_values_flags: DB-verified via types_enum_values _1A0CE18CE7DD4E370B6C0397B3A87332
