#ifndef OBJECT_BEING_DAMAGED_FLAGS_H
#define OBJECT_BEING_DAMAGED_FLAGS_H

/* object_being_damaged_flags - bit indices for the object_being_damaged.flags word.
 * DB-verified: types_enum_values _9CB72D8CE21CA0B0616EC579A54FABE0 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum object_being_damaged_flags {
	_object_being_damaged_body_depleted_bit = 0,
	_object_being_damaged_region_destroyed_bit = 1,
	_object_being_damaged_body_destroyed_bit = 2,
	_object_being_damaged_shield_depleted_bit = 3,
	_object_being_damaged_by_friendly_bit = 4,
	_object_being_damaged_multiplied_by_difficulty_bit = 5,
	_object_being_damaged_killed_instantly_bit = 6,
	_object_being_damaged_force_hard_ping_bit = 7,
	NUMBER_OF_OBJECT_BEING_DAMAGED_FLAGS = 8,
};

#endif /* OBJECT_BEING_DAMAGED_FLAGS_H */

// object_being_damaged_flags: DB-verified via types_enum_values _9CB72D8CE21CA0B0616EC579A54FABE0
