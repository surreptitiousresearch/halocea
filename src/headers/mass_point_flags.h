#pragma once

/* mass_point_flags — enum drain Wave E (2026-08-13): named for the fix sites this wave applies;
 * see the consumer TUs including this header for the per-site evidence.
 * Source: types_enum_values $35EE1D98A1945FD2941159B669ED5A99 (DB-verified, members verbatim in DB order). Values are BIT INDICES — use as (1u << _bit). */
enum mass_point_flags {
	_point_at_rest_bit = 0,
	_point_on_ground_bit = 1,
	_point_on_volatile_surface_bit = 2,
	_point_in_water_bit = 3,
	_point_antigraving_bit = 4,
	NUMBER_OF_POINT_FLAGS = 5,
};

// mass_point_flags: DB-verified via types_enum_values $35EE1D98A1945FD2941159B669ED5A99
