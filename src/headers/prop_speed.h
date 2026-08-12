#pragma once

/* prop_speed — enum drain Wave E (2026-08-13): named for the fix sites this wave applies;
 * see the consumer TUs including this header for the per-site evidence.
 * Source: types_enum_values $2A7E5BA6EC108C666DF6E50CD8BAED3C (DB-verified, members verbatim in DB order). */
enum prop_speed {
	_prop_speed_stopped = 0,
	_prop_speed_walking = 1,
	_prop_speed_running = 2,
	_prop_speed_driving = 3,
	NUMBER_OF_PROP_SPEEDS = 4,
};

// prop_speed: DB-verified via types_enum_values $2A7E5BA6EC108C666DF6E50CD8BAED3C
