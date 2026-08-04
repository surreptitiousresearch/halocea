#ifndef HUD_PANEL_FLAGS_H
#define HUD_PANEL_FLAGS_H

/* hud_panel_flags - bit indices for the hud_panel.flags word.
 * DB-verified: types_enum_values _88CA016EC8DEC4DDE83B82E6CF459496 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum hud_panel_flags {
	_hud_panel_health_dont_show_bit = 0,
	_hud_panel_health_blink_bit = 1,
	_hud_panel_shield_dont_show_bit = 2,
	_hud_panel_shield_blink_bit = 3,
	_hud_panel_motion_sensor_dont_show_bit = 4,
	_hud_panel_motion_sensor_blink_bit = 5,
	NUMBER_OF_SCRIPTED_UNIT_PANELS = 6,
};

#endif /* HUD_PANEL_FLAGS_H */

// hud_panel_flags: DB-verified via types_enum_values _88CA016EC8DEC4DDE83B82E6CF459496
