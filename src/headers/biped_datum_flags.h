#ifndef BIPED_DATUM_FLAGS_H
#define BIPED_DATUM_FLAGS_H

/* biped_datum_flags — bit indices for the RUNTIME _biped_datum.biped.flags word (distinct from the
 * tag-side biped_definition_flags $D3F47BAF). Use as (1u << _bit).
 * No enum survives in headers_ref or the IDA DB; only bit 0 is confidently named:
 *   bit 0 = airborne. Anchors: unit_melee_attack_begin selects animation state
 *   (bit0 set ? 31 _unit_state_melee_airborne : 30 _unit_state_melee_attack);
 *   biped_exit_seat_end sets |1 (you leave a seat airborne); infection_swarm_control skips ground
 *   steering while it is set.
 * Other observed bits, unnamed: bit 1 (set alongside bit 0 by biped_accelerate's |= 3 on physics
 * wake), bit 5 (limp state per the flags-pass audit). */
/* layout bound to DB enum: types_enum_values _7D1C039996C018E08DA24C04FB666BB9 */
enum biped_datum_flags {
	_biped_airborne_bit = 0x0,
	_biped_slipping_bit = 0x1,
	_biped_absolute_movement_bit = 0x2,
	_biped_no_collision_bit = 0x3,
	_biped_movement_passes_through_bipeds_bit = 0x4,
	_biped_limp_body_physics_active_bit = 0x5,
	NUMBER_OF_BIPED_FLAGS = 0x6,
};

#endif /* BIPED_DATUM_FLAGS_H */
