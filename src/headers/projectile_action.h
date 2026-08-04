#ifndef PROJECTILE_ACTION_H
#define PROJECTILE_ACTION_H

/* projectile_datum.action pending-action codes.
 * DB-verified: types_enum_values _895D85001F7E8EBF7832EF04259A825A carries these names verbatim. */
enum projectile_action {
	_projectile_action_none = 0,
	_projectile_action_detonate = 1,
	_projectile_action_disappear = 2,
	NUMBER_OF_PROJECTILE_ACTIONS = 3,
};

#endif /* PROJECTILE_ACTION_H */
