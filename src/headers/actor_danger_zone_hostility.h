#ifndef ACTOR_DANGER_ZONE_HOSTILITY_H
#define ACTOR_DANGER_ZONE_HOSTILITY_H

/* actor_danger_zone_hostility — perceived owner of a noticed danger zone, passed to
 * actor_stimulus_noticed_danger_zone. DB-verified via types_enum_values
 * _201F9ED93A6E09F6C7DD16CDB3EE1AEA. */
enum actor_danger_zone_hostility {
	_actor_danger_hostility_enemy = 0,
	_actor_danger_hostility_friend = 1,
	_actor_danger_hostility_self = 2,
	NUMBER_OF_ACTOR_DANGER_ZONE_HOSTILITIES = 3,
};

#endif /* ACTOR_DANGER_ZONE_HOSTILITY_H */
