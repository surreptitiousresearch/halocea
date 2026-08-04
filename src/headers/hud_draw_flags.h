#ifndef HUD_DRAW_FLAGS_H
#define HUD_DRAW_FLAGS_H

/* hud_draw_flags - bit indices for the hud_draw.flags word.
 * DB-verified: types_enum_values _8B2ECB4D320541E98C17F4C091AF18AE carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum hud_draw_flags {
	_hud_draw_flashing_bit = 0,
	_hud_draw_disabled_bit = 1,
	_hud_draw_in_multiplayer_bit = 2,
	NUMBER_OF_HUD_DRAW_FLAGS = 3,
};

#endif /* HUD_DRAW_FLAGS_H */

// hud_draw_flags: DB-verified via types_enum_values _8B2ECB4D320541E98C17F4C091AF18AE
