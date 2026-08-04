#ifndef HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPE_H
#define HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPE_H

/* hud_multitexture_overlay_effector_type — the `destination_type` field of a
 * multitexture-overlay HUD effector (which channel of the destination is written). */
enum hud_multitexture_overlay_effector_type {
	_hud_multitexture_overlay_effector_type_tint = 0,
	_hud_multitexture_overlay_effector_type_horizontal_offset = 1,
	_hud_multitexture_overlay_effector_type_vertical_offset = 2,
	_hud_multitexture_overlay_effector_type_alpha = 3,
	NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPES = 4,
};

#endif /* HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPE_H */

// hud_multitexture_overlay_effector_type: DB-verified via types_enum_values $3B56896E2CA42FB233930E4FACEAABA3
