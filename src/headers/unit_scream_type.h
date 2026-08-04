#ifndef UNIT_SCREAM_TYPE_H
#define UNIT_SCREAM_TYPE_H

/* unit_scream_type — scream_type argument to unit_scream().
 * DB-verified: types_enum_values _67894A93F3F19A130E6269255EF3739B (anonymous compiled enum). */
enum unit_scream_type {
	_unit_scream_falling = 0,
	_unit_scream_grenade_attached_to_us = 1,
	_unit_scream_burning_to_death = 2,
	_unit_scream_destroyed_limb = 3,
	_unit_scream_destroyed_head = 4,
	_unit_scream_resurrection = 5,
	NUMBER_OF_UNIT_SCREAM_TYPES = 6,
};

#endif /* UNIT_SCREAM_TYPE_H */
