#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

/* event_type — UI input event class (event_record.type, ui event manager queue).
 * Source: reference enum _DC89C1088BBDD1D6E6C5935D1436EA0E (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum event_type {
	event_type_none = 0,
	event_type_left_analog_stick = 1,
	event_type_right_analog_stick = 2,
	event_type_button = 3,
	event_type_mouse_button = 4,
	event_type_custom_activation = 5,
	number_of_event_types = 6
} ;

#endif /* EVENT_TYPE_H */

// event_type: DB-verified via types_enum_values _DC89C1088BBDD1D6E6C5935D1436EA0E (anonymous compiled enum, names sans leading _)
