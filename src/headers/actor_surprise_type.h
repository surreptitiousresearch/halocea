#ifndef ACTOR_SURPRISE_TYPE_H
#define ACTOR_SURPRISE_TYPE_H

/* actor_surprise_type — severity/class of a surprise stimulus registered on an actor
 * (actor_stimulus_data.surprise_level; higher wins). DB-verified via types_enum_values
 * _90931FB8722214B8E7FBE79E1FAD1943. */
enum actor_surprise_type {
	_actor_surprise_none = 0,
	_actor_surprise_unprepared_enemy_shooting = 1,
	_actor_surprise_unprepared_weapon_impact_close = 2,
	_actor_surprise_unprepared_enemy_close = 3,
	_actor_surprise_unprepared_grenade = 4,
	_actor_surprise_unprepared_damage = 5,
	_actor_surprise_unexpected_enemy_close_shooting = 6,
	_actor_surprise_unexpected_enemy_close_flanked_shooting = 7,
	NUMBER_OF_ACTOR_SURPRISE_TYPES = 8,
};

#endif /* ACTOR_SURPRISE_TYPE_H */
