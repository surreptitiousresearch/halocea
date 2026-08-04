#ifndef MULTIPLAYER_VEHICLE_INDEX_H
#define MULTIPLAYER_VEHICLE_INDEX_H

/* multiplayer_vehicle_index — slot order of the game globals multiplayer_information vehicles tag
 * block (each slot a tag_reference to a 'vehi' tag).
 * Source: DB anonymous enum _6BFC23A7E1B3A38EABB96D1BA234D41F (types_enum_values, verbatim). */
enum multiplayer_vehicle_index {
	_multiplayer_vehicle_warthog = 0,
	_multiplayer_vehicle_ghost = 1,
	_multiplayer_vehicle_scorpion = 2,
	_multiplayer_vehicle_banshee = 3,
	_multiplayer_vehicle_c_gun_turret = 4,
	_multiplayer_vehicle_rocket_warthog = 5,
	MULTIPLAYER_MAX_VEHICLES = 6,
};

#endif /* MULTIPLAYER_VEHICLE_INDEX_H */
