#ifndef DIRECTION_SPECIFICATION_TYPE_H
#define DIRECTION_SPECIFICATION_TYPE_H

/* direction_specification_type — actor look/facing direction specification (.type field of
 * direction_specification in look orders). DB-verified via types_enum_values
 * _7170D517B90145C78E764E90D20D8612. */
enum direction_specification_type {
	_direction_specification_movement = 0,
	_direction_specification_prop = 1,
	_direction_specification_target = 2,
	_direction_specification_point = 3,
	_direction_specification_vector = 4,
	_direction_specification_danger = 5,
	_direction_specification_object = 6,
	NUMBER_OF_DIRECTION_SPECIFICATION_TYPES = 7,
};

#endif /* DIRECTION_SPECIFICATION_TYPE_H */
