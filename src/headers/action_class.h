#ifndef ACTION_CLASS_H
#define ACTION_CLASS_H

/* action_class — value of action_specification.action_class (the behaviour category of an
 * actor action). Source: types_enum_values _20817E28DA5EBB5C3934C172FAB76C80 (DB-verified). */
enum action_class {
	_action_class_noncombat = 0,
	_action_class_passive = 1,
	_action_class_transitory = 2,
	_action_class_pursuit = 3,
	_action_class_active = 4,
	NUMBER_OF_ACTION_CLASSES = 5,
};

#endif /* ACTION_CLASS_H */
