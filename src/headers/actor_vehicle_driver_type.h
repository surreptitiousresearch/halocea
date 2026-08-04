#ifndef ACTOR_VEHICLE_DRIVER_TYPE_H
#define ACTOR_VEHICLE_DRIVER_TYPE_H

/* actor_vehicle_driver_type — value space of actor input.vehicle_driver_type (__int16 @+62).
 * DB-verified: types_enum_values _5FC149AD366E97BDBF568DBE8E3C2DFC (anonymous compiled enum). */
enum actor_vehicle_driver_type {
	_actor_vehicle_driver_none = 0,
	_actor_vehicle_driver_unknown = 1,
	_actor_vehicle_driver_hovering_ground = 2,
	_actor_vehicle_driver_nondirectional_ground = 3,
	_actor_vehicle_driver_directional_flying = 4,
	NUMBER_OF_ACTOR_VEHICLE_DRIVER_TYPES = 5,
};

#endif /* ACTOR_VEHICLE_DRIVER_TYPE_H */
