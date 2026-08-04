#ifndef PRIMARY_LOOK_PRIORITY_H
#define PRIMARY_LOOK_PRIORITY_H

/* primary_look_priority — actor primary look/facing priority (look_orders primary_priority).
 * DB-verified via types_enum_values _23CF6415980CA92983E74CD03226C694. */
enum primary_look_priority {
	_primary_priority_none = 0,
	_primary_priority_face_360 = 1,
	_primary_priority_opportunity_aiming = 2,
	_primary_priority_facing = 3,
	_primary_priority_exact_facing = 4,
	_primary_priority_aiming = 5,
	_primary_priority_locked_facing = 6,
	_primary_priority_locked_aiming = 7,
	NUMBER_OF_PRIMARY_LOOK_TYPES = 8,
};

#endif /* PRIMARY_LOOK_PRIORITY_H */
