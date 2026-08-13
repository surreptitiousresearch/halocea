#ifndef BIPED_DATUM_FLAGS_H
#define BIPED_DATUM_FLAGS_H

/* biped_datum_flags — bit indices for the RUNTIME _biped_datum.biped.flags word (distinct from the
 * tag-side biped_definition_flags $D3F47BAF). Use as (1u << _bit).
 * All 6 bits are DB-named via types_enum_values $7D1C0399 (the earlier "no enum survives /
 * bits 1 and 5 unnamed" prose predated the enum extraction and was stale — corrected
 * 2026-08-13). Behavioral anchors still hold: bit 0 airborne (unit_melee_attack_begin picks
 * _unit_state_melee_airborne on it; biped_exit_seat_end sets it; infection_swarm_control
 * skips ground steering on it); bits 0|1 set together by biped_accelerate on physics wake;
 * bit 5 = limp-body physics. */
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
