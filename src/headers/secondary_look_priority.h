#ifndef SECONDARY_LOOK_PRIORITY_H
#define SECONDARY_LOOK_PRIORITY_H

/* secondary_look_priority — actor secondary look priority (look_orders secondary_priority).
 * DB-verified via types_enum_values _27054D89B39C4945D5898B7F89C473DB. */
enum secondary_look_priority {
	_secondary_look_priority_none = 0,
	_secondary_look_priority_default = 1,
	_secondary_look_priority_idle_look = 2,
	_secondary_look_priority_idle_aim = 3,
	_secondary_look_priority_aim = 4,
	_secondary_look_priority_turn_and_aim = 5,
	_secondary_look_priority_stop_and_aim = 6,
	_secondary_look_priority_override = 7,
	_secondary_look_priority_override_exact_facing = 8,
	NUMBER_OF_SECONDARY_LOOK_PRIORITIES = 9,
};

#endif /* SECONDARY_LOOK_PRIORITY_H */
