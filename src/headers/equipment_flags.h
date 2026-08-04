#ifndef EQUIPMENT_FLAGS_H
#define EQUIPMENT_FLAGS_H

/* equipment_flags - bit indices for the equipment.flags word.
 * DB-verified: types_enum_values _E7D11F01F56DDF73B4AFE1CE8091A96A carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum equipment_flags {
	_equipment_created_at_rest_bit = 0,
	_equipment_obsolete_bit = 1,
	_equipment_does_accelerate_bit = 2,
	NUMBER_OF_SCENARIO_EQUIPMENT_FLAGS = 3,
};

#endif /* EQUIPMENT_FLAGS_H */

// equipment_flags: DB-verified via types_enum_values _E7D11F01F56DDF73B4AFE1CE8091A96A
