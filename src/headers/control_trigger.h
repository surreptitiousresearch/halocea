#ifndef CONTROL_TRIGGER_H
#define CONTROL_TRIGGER_H

/* control_trigger — value of control_definition.trigger ("triggers when" for a device
 * control). Source: types_enum_values $D92CD89B9AB938B20FFE040D02ED028E (DB-verified). */
enum control_trigger {
	_control_trigger_player = 0,
	_control_trigger_destruction = 1,
	NUMBER_OF_CONTROL_TRIGGERS = 2,
};

#endif /* CONTROL_TRIGGER_H */
