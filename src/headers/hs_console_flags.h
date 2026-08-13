#ifndef HS_CONSOLE_FLAGS_H
#define HS_CONSOLE_FLAGS_H

/* hs_console_flags — bit indices for the console token-enumeration availability mask.
 * DB-verified: types_enum_values _B9FE84D7C743149CB3320C212DA0A1B8 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit).
 * get_console_test_flags pairs each low bit N (context allows) with a high bit N+8
 * (context explicitly disallows); the disallow bit strips the paired allow bit. */
enum hs_console_flags {
	_hs_console_allowed = 0,
	_hs_console_not_allowed = 0,
	_hs_console_allowed_default = 0,
	_hs_console_listen_server = 1,
	_hs_console_dedicated_server = 2,
	_hs_console_ingame = 3,
	_hs_console_idle = 4,
	_hs_console_cheating = 5,
	_hs_console_ship_game = 6,
	_hs_console_flag_count = 7,
};

/* shift applied to a flag index to get its paired "disallow" bit.
 * ATTESTED: get_console_test_flags 0x83722280-0x83722320 is 7 consecutive
 * (test bit N+8 -> clear bit N) pairs, N=0..6 = the whole enum domain. */
#define HS_CONSOLE_DISALLOW_SHIFT 8

#endif /* HS_CONSOLE_FLAGS_H */

// hs_console_flags: DB-verified via types_enum_values _B9FE84D7C743149CB3320C212DA0A1B8
