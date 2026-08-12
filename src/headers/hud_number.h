#pragma once

/* hud_number — the non-digit glyph FRAME indices of the shared "hud digits" bitmap group
 * (_interface_hud_digits, see hud_number_definition.h). Frames 0..9 are the digits themselves and
 * are drawn by value, so the DB names only the five punctuation/suffix cells. hud_draw_numbers
 * (@0x8379FB90) draws frame 10 for the decimal point and frame 12 for the minus sign, and picks
 * 14 vs 13 for the suffix of a "show trailing m" readout — the >= 1000 case divides the value by
 * 1000 and takes 14 (kilometres), the < 1000 case takes 13 (metres). Source: types_enum_values
 * $65F9E4279B0BB0AB343D753EBFE6DE81 (DB-verified). */
enum hud_number {
	_hud_number_decimal_index = 10,
	_hud_number_colon_index = 11,
	_hud_number_negative_sign_index = 12,
	_hud_number_meters_index = 13,
	_hud_number_kilometers_index = 14,
};

// hud_number: DB-verified via types_enum_values $65F9E4279B0BB0AB343D753EBFE6DE81
