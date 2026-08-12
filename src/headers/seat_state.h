#pragma once

/* seat_state — enum drain Wave E (2026-08-13): named for the fix sites this wave applies;
 * see the consumer TUs including this header for the per-site evidence.
 * Source: types_enum_values $DA15F6194ACDE9ACFB5DB64BF9A621EA (DB-verified, members verbatim in DB order). */
enum seat_state {
	_not_in_seat = 0,
	_entering_seat = 1,
	_seat_idle = 2,
	_exiting_seat = 3,
};

// seat_state: DB-verified via types_enum_values $DA15F6194ACDE9ACFB5DB64BF9A621EA
