#ifndef NETGAME_FLAG_TYPE_H
#define NETGAME_FLAG_TYPE_H

/* netgame_flag_type — scenario_netgame_flag.type (__int16 @0x10).
 * DB-verified: types_enum_values _2CF2ECB13EA77347116BA5759ED0A245 carries these verbatim. */
enum netgame_flag_type {
	_netgame_flag_ctf_flag = 0,
	_netgame_flag_ctf_vehicle = 1,
	_netgame_flag_oddball_ball_spawn = 2,
	_netgame_flag_race_track = 3,
	_netgame_flag_race_vehicle = 4,
	_netgame_flag_vegas_bank = 5,
	_netgame_flag_teleporter_source = 6,
	_netgame_flag_teleporter_target = 7,
	_netgame_flag_hill = 8,
	NUMBER_OF_NETGAME_FLAG_TYPES = 9,
};

#endif /* NETGAME_FLAG_TYPE_H */

// netgame_flag_type: DB-verified via types_enum_values _2CF2ECB13EA77347116BA5759ED0A245
