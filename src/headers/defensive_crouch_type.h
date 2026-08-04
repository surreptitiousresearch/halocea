#ifndef DEFENSIVE_CROUCH_TYPE_H
#define DEFENSIVE_CROUCH_TYPE_H

/* actor_defensive_properties.defensive_crouch_type domain.
 * DB anonymous enum _26B9A3900CBD53D940F1674E51DA6374. */
enum defensive_crouch_type {
	_defensive_crouch_none = 0,
	_defensive_crouch_danger = 1,
	_defensive_crouch_shield_low = 2,
	_defensive_crouch_hide_behind_shield = 3,
	_defensive_crouch_any_target = 4,
	_defensive_crouch_flood_shamble = 5,
	NUMBER_OF_DEFENSIVE_CROUCH_TYPES = 6,
};

#endif /* DEFENSIVE_CROUCH_TYPE_H */
