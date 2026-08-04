#ifndef HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS_H
#define HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS_H

/* hud_screen_effect_light_enhancement_flags — hud_screen_effect_light_enhancement flags.
 * DB-verified: types_enum_values _2AF85CC73EC0B52AA1B9365791E8B926 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum hud_screen_effect_light_enhancement_flags {
	_hud_screen_effect_light_enhancement_only_when_zoomed_bit = 0,
	_hud_screen_effect_light_enhancement_connect_to_flashlight_bit = 1,
	_hud_screen_effect_light_enhancement_uses_convolution_mask_bit = 2,
	NUMBER_OF_HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS = 3,
};

#endif /* HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS_H */

// hud_screen_effect_light_enhancement_flags: DB-verified via types_enum_values _2AF85CC73EC0B52AA1B9365791E8B926
