#ifndef SECONDARY_LOOK_TYPE_H
#define SECONDARY_LOOK_TYPE_H

/* secondary_look_type — stimulus class that triggered an actor secondary look.
 * DB-verified via types_enum_values _4FE2BC9FABCC35AAFF9CB0F0040EF509. */
enum secondary_look_type {
	_secondary_look_none = 0,
	_secondary_look_environmental_noise = 1,
	_secondary_look_started_moving_prop = 2,
	_secondary_look_weapon_impact = 3,
	_secondary_look_newly_acknowledged_prop = 4,
	_secondary_look_bumped_prop = 5,
	_secondary_look_detonation = 6,
	_secondary_look_shooting_prop = 7,
	_secondary_look_communicating_prop = 8,
	_secondary_look_communicated_direction = 9,
	_secondary_look_combat_stimulus_prop = 10,
	_secondary_look_damage = 11,
	_secondary_look_dangerous_object = 12,
	_secondary_look_scripted = 13,
	NUMBER_OF_SECONDARY_LOOK_TYPES = 14,
};

#endif /* SECONDARY_LOOK_TYPE_H */
