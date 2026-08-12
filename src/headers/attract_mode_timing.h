#pragma once

/* attract_mode_timing — enum drain Wave E (2026-08-13): named for the fix sites this wave applies;
 * see the consumer TUs including this header for the per-site evidence.
 * Source: types_enum_values $BD348CA3857D7E228514BE6BDA8EA374 (DB-verified, members verbatim in DB order). */
enum attract_mode_timing {
	ATTRACT_MODE_COUNTDOWN = 75000,
	MUSIC_FADE_TIME = 1500,
};

// attract_mode_timing: DB-verified via types_enum_values $BD348CA3857D7E228514BE6BDA8EA374
