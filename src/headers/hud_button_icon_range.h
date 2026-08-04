#ifndef HUD_BUTTON_ICON_RANGE_H
#define HUD_BUTTON_ICON_RANGE_H

/* hud_button_icon_range — the byte ranges of the packed HUD-message special-element codes:
 * 0..17 specific button icons (direct index), 18..31 remapped controller buttons (index via the
 * player's control mapping), 32..39 misc icon / scenario-name slots (code - 32 selects the slot). */
enum hud_button_icon_range {
	_hud_icon_specific_button_start = 0,
	NUMBER_OF_HUD_CUSTOM_ICONS = 8,
	_hud_icon_specific_button_end = 17,
	_hud_icon_remapped_button_start = 18,
	_hud_icon_remapped_button_end = 31,
	_hud_icon_misc_start = 32,
	NUMBER_OF_HUD_BUTTON_ICONS = 32,
	_hud_icon_misc_end = 39,
	NUMBER_OF_HUD_ICON_TYPES = 40,
};

#endif /* HUD_BUTTON_ICON_RANGE_H */

// hud_button_icon_range: DB-verified via types_enum_values $9B8B9C52EE51CF6B892253F06A72134A
