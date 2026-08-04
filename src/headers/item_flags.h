#ifndef ITEM_FLAGS_H
#define ITEM_FLAGS_H

/* item_flags - bit indices for the item.flags word.
 * DB-verified: types_enum_values _982827FBA02489028BA02B996F8FF3A9 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum item_flags {
	_item_in_unit_inventory_bit = 0,
	_item_belongs_to_player_bit = 1,
	_item_has_nonzero_angular_velocity_bit = 2,
	_item_on_structure_bit = 3,
	_item_on_object_bit = 4,
	_item_does_not_accelerate_bit = 5,
	_item_part_of_respawn_system = 6,
	NUMBER_OF_ITEM_DATUM_FLAGS = 7,
};

#endif /* ITEM_FLAGS_H */

// item_flags: DB-verified via types_enum_values _982827FBA02489028BA02B996F8FF3A9
