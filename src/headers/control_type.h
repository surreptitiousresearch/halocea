#ifndef CONTROL_TYPE_H
#define CONTROL_TYPE_H

/* control_type — values of control_definition.control.type (device_control tag "type" field).
 * DB-verified via types_enum_values $550C8D45ACBCD4A90DD32FFBFC5E0F93. */
enum control_type {
	_control_toggle_switch = 0,
	_control_on_button = 1,
	_control_off_button = 2,
	_control_call_button = 3,
	NUMBER_OF_CONTROL_TYPES = 4,
};

#endif /* CONTROL_TYPE_H */
