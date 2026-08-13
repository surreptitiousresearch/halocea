#pragma once

/* cseries_limits — the cseries integer-limit constants (Blam spellings of SHORT_MAX/LONG_MIN et al.).
 * Source: types_enum_values $D48D356A7F7AA2832FFEAC21499B0F33 (DB-verified, members verbatim in DB order). */
enum cseries_limits {
	UNSIGNED_LONG_MAX = -1,
	LONG_MAX = 2147483647,
	LONG_MIN = -2147483648,
	LONG_BITS = 32,
	LONG_BITS_BITS = 5,
	UNSIGNED_SHORT_MAX = 65535,
	SHORT_MAX = 32767,
	SHORT_MIN = -32768,
	SHORT_BITS = 16,
	SHORT_BITS_BITS = 4,
	UNSIGNED_CHAR_MAX = 255,
	CHAR_MAX = 127,
	CHAR_MIN = -128,
	CHAR_BITS = 8,
	CHAR_BITS_BITS = 3,
};

// cseries_limits: DB-verified via types_enum_values $D48D356A7F7AA2832FFEAC21499B0F33
