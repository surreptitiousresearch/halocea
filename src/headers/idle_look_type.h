#ifndef IDLE_LOOK_TYPE_H
#define IDLE_LOOK_TYPE_H

/* idle_look_type — actor idle-look behavior class (look_orders idle_look_type).
 * DB-verified via types_enum_values _8E0C545567762B53C03B58F010C12FC4. */
enum idle_look_type {
	_idle_look_none = 0,
	_idle_look_noncombat = 1,
	_idle_look_guarding = 2,
	_idle_look_searching = 3,
	_idle_look_combat = 4,
	NUMBER_OF_IDLE_LOOK_TYPES = 5,
};

#endif /* IDLE_LOOK_TYPE_H */
