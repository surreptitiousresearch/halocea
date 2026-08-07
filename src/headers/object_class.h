#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

/* object_class — the two object cluster-partition classes (collideable/noncollideable).
 * DB-verified: types_enum_values $8B2FAA36BBE2558D9CF177EE1F018481 carries these names verbatim.
 * Values are BIT INDICES into the class_flags word — use as (1u << _object_class_*). */
enum object_class {
	_object_class_collideable = 0,
	_object_class_noncollideable = 1,
	NUMBER_OF_OBJECT_CLASSES = 2,
};

#endif /* OBJECT_CLASS_H */

// object_class: DB-verified via types_enum_values _8B2FAA36BBE2558D9CF177EE1F018481
