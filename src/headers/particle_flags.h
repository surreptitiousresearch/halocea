#ifndef PARTICLE_FLAGS_H
#define PARTICLE_FLAGS_H

/* particle_flags — particle flags.
 * DB-verified: types_enum_values _4B7E5BF20A63198B5F78B9869C72B50F carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum particle_flags {
	_particle_animates_backwards_bit = 0,
	_particle_at_rest_bit = 1,
	_particle_u_mirror_bit = 2,
	_particle_v_mirror_bit = 3,
	_particle_dont_draw_first_person_bit = 4,
	_particle_dont_draw_third_person_bit = 5,
	_particle_attached_to_local_player_bit = 6,
	NUMBER_OF_PARTICLE_FLAGS = 7,
};

#endif /* PARTICLE_FLAGS_H */

// particle_flags: DB-verified via types_enum_values _4B7E5BF20A63198B5F78B9869C72B50F
