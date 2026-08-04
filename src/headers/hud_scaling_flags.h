#ifndef HUD_SCALING_FLAGS_H
#define HUD_SCALING_FLAGS_H

/* hud_scaling_flags — bit indices for the hud_scaling flags word.
 * DB-verified: types_enum_values _0A0128C9F2DB9D2BBA6B0E70252C363C carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum hud_scaling_flags {
	_hud_dont_scale_offset_bit = 0,
	_hud_dont_scale_size_bit = 1,
	_hud_use_high_resolution_scale_bit = 2,
	NUMBER_OF_HUD_SCALING_FLAGS = 3,
};

#endif /* HUD_SCALING_FLAGS_H */

// hud_scaling_flags: DB-verified via types_enum_values _0A0128C9F2DB9D2BBA6B0E70252C363C
