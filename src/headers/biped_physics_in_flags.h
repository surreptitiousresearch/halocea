#ifndef BIPED_PHYSICS_IN_FLAGS_H
#define BIPED_PHYSICS_IN_FLAGS_H

/* biped_physics_in_flags — biped_physics_in flags.
 * DB-verified: types_enum_values _6B43552B227A446356DBBB5698AE8A21 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum biped_physics_in_flags {
	_biped_physics_in_airborne_bit = 0,
	_biped_physics_in_slipping_bit = 1,
	_biped_physics_in_crouched_bit = 2,
	_biped_physics_in_trying_to_stand_bit = 3,
	_biped_physics_in_flying_bit = 4,
	_biped_physics_in_absolute_movement_bit = 5,
	_biped_physics_in_no_collision_bit = 6,
	_biped_physics_in_dead_bit = 7,
	_biped_physics_in_pass_through_bipeds_bit = 8,
	_biped_physics_in_climb_anything_bit = 9,
	NUMBER_OF_BIPED_PHYSICS_IN_FLAGS = 10,
};

#endif /* BIPED_PHYSICS_IN_FLAGS_H */

// biped_physics_in_flags: DB-verified via types_enum_values _6B43552B227A446356DBBB5698AE8A21
