#ifndef ENCOUNTER_FLAGS_H
#define ENCOUNTER_FLAGS_H

/* encounter_flags — encounter flags.
 * DB-verified: types_enum_values _2CF0A8C02891F3ADE7B10560F0A4F8E9 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum encounter_flags {
	_encounter_not_initially_placed_bit = 0,
	_encounter_respawn_enable_bit = 1,
	_encounter_blind_bit = 2,
	_encounter_deaf_bit = 3,
	_encounter_braindead_bit = 4,
	_encounter_3d_firing_positions_bit = 5,
	_encounter_manual_structure_bsp_bit = 6,
	NUMBER_OF_ENCOUNTER_FLAGS = 7,
};

#endif /* ENCOUNTER_FLAGS_H */

// encounter_flags: DB-verified via types_enum_values _2CF0A8C02891F3ADE7B10560F0A4F8E9
