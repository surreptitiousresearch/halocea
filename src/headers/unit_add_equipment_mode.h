#ifndef UNIT_ADD_EQUIPMENT_MODE_H
#define UNIT_ADD_EQUIPMENT_MODE_H

/* Mode argument of unit_add_equipment_to_inventory.
 * DB-verified via types_enum_values _1A2640EBA3F8FB1D437768CF433C8A39 (names verbatim). */
enum unit_add_equipment_mode {
	_unit_add_equipment_normally = 0,
	_unit_add_equipment_as_only_equipment = 1,
	NUMBER_OF_UNIT_ADD_EQUIPMENT_MODES = 2,
};

#endif /* UNIT_ADD_EQUIPMENT_MODE_H */
