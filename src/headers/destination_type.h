#ifndef DESTINATION_TYPE_H
#define DESTINATION_TYPE_H

/* destination_type - AI path destination_orders.destination_type / orders.move.destination. */
enum destination_type {
	_destination_none = 0,
	_destination_halt = 1,
	_destination_raw_location = 2,
	_destination_firing_position = 3,
	_destination_move_position = 4,
	_destination_prop = 5,
	NUMBER_OF_DESTINATION_TYPES = 6,
};

#endif /* DESTINATION_TYPE_H */

// destination_type: DB-verified via types_enum_values _849D342FFE0EA16DE7A3D41C85BCF8CB (anonymous compiled enum, names sans leading _)
