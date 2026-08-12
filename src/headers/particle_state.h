#pragma once

/* particle_state — the value domain of particle_datum.state (0x0E, uint8_t), the phase of a
 * particle's walk through its definition's birth -> loop -> death bitmap-sequence ranges.
 * particle_next_sequence @0x8373DE10 is the whole state machine: state 0 picks a sequence
 * from the initial range and steps to 1; state 1 with no sequence yet drops to 2; state 2
 * picks from the looping range until life_time reaches life_span (then steps to 3); state 3
 * picks from the final range and steps to 4, after which no sequence remains and the
 * particle dies. particle_new @0x8373E808 seeds it to 0. The five DB names map onto that
 * arc position for position, and NUMBER_OF_PARTICLE_STATES = 5 matches the 0..4 walk.
 * Distinct from particle_system_type_particle_state (a tag-block struct) and from the
 * particle_states block index in the particle_system code.
 * Source: types_enum_values $5122974F880A61B29072C95E8284800C (DB-verified). */
enum particle_state {
	_particle_state_next_sequence_initial = 0,
	_particle_state_next_sequence_looping = 1,
	_particle_state_still_looping = 2,
	_particle_state_next_sequence_final = 3,
	_particle_state_final_sequence = 4,
	NUMBER_OF_PARTICLE_STATES = 5,
};

// particle_state: DB-verified via types_enum_values $5122974F880A61B29072C95E8284800C
