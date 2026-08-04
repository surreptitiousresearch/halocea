#ifndef HUD_SCREEN_EFFECT_DESATURATION_FLAGS_H
#define HUD_SCREEN_EFFECT_DESATURATION_FLAGS_H

/* hud_screen_effect_desaturation_flags — bit indices for the hud_screen_effect_desaturation flags word.
 * DB-verified: types_enum_values _0078446F1CEC641F834F4D32011ADE86 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum hud_screen_effect_desaturation_flags {
	_hud_screen_effect_desaturation_only_when_zoomed_bit = 0,
	_hud_screen_effect_desaturation_connect_to_flashlight_bit = 1,
	_hud_screen_effect_desaturation_is_additive_bit = 2,
	_hud_screen_effect_desaturation_uses_convolution_mask_bit = 3,
	NUMBER_OF_HUD_SCREEN_EFFECT_DESATURATION_FLAGS = 4,
};

#endif /* HUD_SCREEN_EFFECT_DESATURATION_FLAGS_H */

// hud_screen_effect_desaturation_flags: DB-verified via types_enum_values _0078446F1CEC641F834F4D32011ADE86
