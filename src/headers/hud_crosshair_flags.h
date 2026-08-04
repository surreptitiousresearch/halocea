#ifndef HUD_CROSSHAIR_FLAGS_H
#define HUD_CROSSHAIR_FLAGS_H

/* hud_crosshair_flags — bit indices for weapon_hud_crosshair_item.flags.
 * DB-verified: types_enum_values _67437FDD618E30FAFDE38D8F838000CC carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (flags >> _bit) & 1. */
enum hud_crosshair_flags {
	_hud_crosshair_flashes_bit = 0,
	_hud_crosshair_not_a_sprite_bit = 1,
	_hud_crosshair_not_on_default_zoom_bit = 2,
	_hud_crosshair_show_sniper_data_bit = 3,
	_hud_crosshair_hide_outside_area_bit = 4,
	_hud_crosshair_one_zoom_level_bit = 5,
	_hud_crosshair_only_on_default_zoom_bit = 6,
	_hud_crosshair_runtime_invalid_bit = 7,  /* DB-verified: value 7 (was wrongly 8 here) */
};

#endif /* HUD_CROSSHAIR_FLAGS_H */

// hud_crosshair_flags: DB-verified via types_enum_values _67437FDD618E30FAFDE38D8F838000CC
