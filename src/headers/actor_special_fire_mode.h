#ifndef ACTOR_SPECIAL_FIRE_MODE_H
#define ACTOR_SPECIAL_FIRE_MODE_H

/* actor_special_fire_mode — values for actor_variant_ranged_combat.special_fire_mode.
 * DB-verified via types_enum_values $5BC8143B5D97FE75AE109E3FE5F5E285. */
enum actor_special_fire_mode {
	_actor_special_fire_mode_none = 0,
	_actor_special_fire_mode_overcharge = 1,
	_actor_special_fire_mode_secondary = 2,
	NUMBER_OF_ACTOR_SPECIAL_FIRE_MODES = 3,
};

#endif /* ACTOR_SPECIAL_FIRE_MODE_H */
