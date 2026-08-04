#ifndef HUD_WEAPON_CROSSHAIR_FLAGS_H
#define HUD_WEAPON_CROSSHAIR_FLAGS_H

/* hud_weapon_crosshair_flags — hud_weapon_crosshair flags.
 * DB-verified: types_enum_values _67437FDD618E30FAFDE38D8F838000CC carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum hud_weapon_crosshair_flags {
	_hud_crosshair_flashes_bit = 0,
	_hud_crosshair_not_a_sprite_bit = 1,
	_hud_crosshair_not_on_default_zoom_bit = 2,
	_hud_crosshair_show_sniper_data_bit = 3,
	_hud_crosshair_hide_outside_area_bit = 4,
	_hud_crosshair_one_zoom_level_bit = 5,
	_hud_crosshair_only_on_default_zoom_bit = 6,
	NUMBER_OF_HUD_WEAPON_CROSSHAIR_FLAGS = 7,
	_hud_crosshair_runtime_invalid_bit = 7,
	NUMBER_OF_HUD_WEAPON_CROSSHAIR_RUNTIME_FLAGS = 8,
};

#endif /* HUD_WEAPON_CROSSHAIR_FLAGS_H */

// hud_weapon_crosshair_flags: DB-verified via types_enum_values _67437FDD618E30FAFDE38D8F838000CC
