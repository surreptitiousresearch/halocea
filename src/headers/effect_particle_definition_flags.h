#ifndef EFFECT_PARTICLE_DEFINITION_FLAGS_H
#define EFFECT_PARTICLE_DEFINITION_FLAGS_H

/* effect_particle_definition_flags — effect_particle_definition flags.
 * DB-verified: types_enum_values _29C1E338D8F13B256069893A5B79FF14 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum effect_particle_definition_flags {
	_effect_particle_attached_bit = 0,
	_effect_particle_random_orientation_bit = 1,
	_effect_particle_tint_from_change_color_bit = 2,
	_effect_particle_tint_interpolate_hsv_bit = 3,
	_effect_particle_tint_do_it_the_hard_way_bit = 4,
	NUMBER_OF_EFFECT_PARTICLE_DEFINITION_FLAGS = 5,
};

#endif /* EFFECT_PARTICLE_DEFINITION_FLAGS_H */

// effect_particle_definition_flags: DB-verified via types_enum_values _29C1E338D8F13B256069893A5B79FF14
