#pragma once

/* loading_screen_state — enum drain Wave E (2026-08-13): named for the fix sites this wave applies;
 * see the consumer TUs including this header for the per-site evidence.
 * Source: types_enum_values $863583B3767790E589EE57CA82B7336A (DB-verified, members verbatim in DB order). */
enum loading_screen_state {
	LOADING_STATE_OFF = 0,
	LOADING_STATE_SOLO = 1,
	LOADING_STATE_SERVER = 2,
	LOADING_STATE_RESOLVING = 3,
	LOADING_STATE_NEGOTIATING = 4,
	LOADING_STATE_CONNECTING = 5,
	LOADING_STATE_RETRYING = 6,
	LOADING_STATE_CONNECTED = 7,
	LOADING_STATE_LOADING_MP_MAP = 8,
	LOADING_STATE_WAITING_FOR_NEW_GAME = 9,
	LOADING_STATE_MAX_STATES = 10,
};

// loading_screen_state: DB-verified via types_enum_values $863583B3767790E589EE57CA82B7336A
