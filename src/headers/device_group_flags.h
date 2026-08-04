#ifndef DEVICE_GROUP_FLAGS_H
#define DEVICE_GROUP_FLAGS_H

/* device_group_flags - bit indices for the device_group.flags word.
 * DB-verified: types_enum_values _76E083524A6DAB116852662C22ECB38A carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum device_group_flags {
	_device_group_can_change_only_once_bit = 0,
	_device_group_changed_once_bit = 1,
	_device_group_runtime_bit = 2,
	NUMBER_OF_DEVICE_GROUP_FLAGS = 3,
};

#endif /* DEVICE_GROUP_FLAGS_H */

// device_group_flags: DB-verified via types_enum_values _76E083524A6DAB116852662C22ECB38A
