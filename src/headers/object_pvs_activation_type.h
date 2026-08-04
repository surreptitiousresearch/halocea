#ifndef OBJECT_PVS_ACTIVATION_TYPE_H
#define OBJECT_PVS_ACTIVATION_TYPE_H

/* How the object PVS (potentially visible set) activation focus is specified:
 * normal (camera), pinned to an object, or pinned to a cluster. */
enum object_pvs_activation_type {
	_pvs_activation_normal = 0,
	_pvs_activation_object = 1,
	_pvs_activation_cluster = 2,
	NUMBER_OF_OBJECT_PVS_ACTIVATION_TYPES = 3,
};

#endif /* OBJECT_PVS_ACTIVATION_TYPE_H */

// object_pvs_activation_type: DB-verified via types_enum_values $24716EF6C4342B803A17B135AC3AD892
