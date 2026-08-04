#ifndef DEVICE_CHANGE_FLAGS_H
#define DEVICE_CHANGE_FLAGS_H

/* device_change_flags — device_change flags.
 * DB-verified: types_enum_values _524099028B0296877C88D895D3992EC9 carries these named
 * values verbatim. Bit-index members use as (1u << _bit); *_MASK / *_MAXIMUM
 * / large members are composite masks or constants, used as-is. */
enum device_change_flags {
	_device_change_controller0_removed_bit = 0,
	_device_change_controller1_removed_bit = 1,
	_device_change_controller2_removed_bit = 2,
	_device_change_controller3_removed_bit = 3,
	_device_change_mutop0_removed_bit = 4,
	_device_change_mubottom0_removed_bit = 5,
	_device_change_mutop1_removed_bit = 6,
	_device_change_mubottom1_removed_bit = 7,
	_device_change_mutop2_removed_bit = 8,
	_device_change_mubottom2_removed_bit = 9,
	_device_change_mutop3_removed_bit = 10,
	_device_change_mubottom3_removed_bit = 11,
	_device_change_controller0_inserted_bit = 12,
	_device_change_controller1_inserted_bit = 13,
	_device_change_controller2_inserted_bit = 14,
	_device_change_controller3_inserted_bit = 15,
	_device_change_mutop0_inserted_bit = 16,
	_device_change_mubottom0_inserted_bit = 17,
	_device_change_mutop1_inserted_bit = 18,
	_device_change_mubottom1_inserted_bit = 19,
	_device_change_mutop2_inserted_bit = 20,
	_device_change_mubottom2_inserted_bit = 21,
	_device_change_mutop3_inserted_bit = 22,
	_device_change_mubottom3_inserted_bit = 23,
	NUMBER_OF_DEVICE_CHANGE_FLAGS = 24,
	DEVICES_REMOVED = 4095,
	DEVICES_INSERTED = 16773120,
};

#endif /* DEVICE_CHANGE_FLAGS_H */

// device_change_flags: DB-verified via types_enum_values _524099028B0296877C88D895D3992EC9
