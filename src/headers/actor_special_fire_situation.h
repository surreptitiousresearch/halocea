#ifndef ACTOR_SPECIAL_FIRE_SITUATION_H
#define ACTOR_SPECIAL_FIRE_SITUATION_H

/* actor_special_fire_situation — values for actor_variant_ranged_combat.special_fire_situation.
 * DB-verified: types_enum_values _51430FC9FBF026586C2F9221CC75D569 carries these named values verbatim. */
enum actor_special_fire_situation {
	_actor_special_fire_situation_never = 0,
	_actor_special_fire_situation_enemy_visible = 1,
	_actor_special_fire_situation_enemy_out_of_sight = 2,
	_actor_special_fire_situation_strafing = 3,
	NUMBER_OF_ACTOR_SPECIAL_FIRE_SITUATIONS = 4,
};

#endif /* ACTOR_SPECIAL_FIRE_SITUATION_H */
