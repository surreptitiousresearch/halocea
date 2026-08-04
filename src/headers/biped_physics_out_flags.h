#ifndef BIPED_PHYSICS_OUT_FLAGS_H
#define BIPED_PHYSICS_OUT_FLAGS_H

/* biped_physics_out_flags - bit indices for the biped_physics_out.flags word.
 * DB-verified: types_enum_values _8BAF9605717DA8F6063317BE791A457F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum biped_physics_out_flags {
	_biped_physics_out_airborne_bit = 0,
	_biped_physics_out_slipping_bit = 1,
	_biped_physics_out_cannot_stand_bit = 2,
	_biped_physics_out_splatter_bit = 3,
	_biped_physics_out_volatile_collision_bit = 4,
	NUMBER_OF_BIPED_PHYSICS_OUT_FLAGS = 5,
};

#endif /* BIPED_PHYSICS_OUT_FLAGS_H */

// biped_physics_out_flags: DB-verified via types_enum_values _8BAF9605717DA8F6063317BE791A457F
