#ifndef HUD_NUMBER_SHOW_FLAGS_H
#define HUD_NUMBER_SHOW_FLAGS_H

/* hud_number_show_flags - bit indices for the hud_number_show.flags word.
 * DB-verified: types_enum_values _767E50F81A0207F55F3347C1EC3FDF12 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum hud_number_show_flags {
	_hud_number_show_all_leading_zeros_bit = 0,
	_hud_number_show_only_when_zoomed = 1,
	_hud_number_show_trailing_m = 2,
	NUMBER_OF_HUD_NUMBER_FLAGS = 3,
};

#endif /* HUD_NUMBER_SHOW_FLAGS_H */

// hud_number_show_flags: DB-verified via types_enum_values _767E50F81A0207F55F3347C1EC3FDF12
