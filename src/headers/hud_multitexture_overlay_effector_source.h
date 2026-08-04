#ifndef HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_H
#define HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_H

/* hud_multitexture_overlay_effector_source — the `source` field of a multitexture-overlay HUD
 * effector (what live value drives the effector input). */
enum hud_multitexture_overlay_effector_source {
	_hud_multitexture_overlay_effector_source_player_pitch = 0,
	_hud_multitexture_overlay_effector_source_player_pitch_tangent = 1,
	_hud_multitexture_overlay_effector_source_player_yaw = 2,
	_hud_multitexture_overlay_effector_source_weapon_ammo_loaded = 3,
	_hud_multitexture_overlay_effector_source_weapon_ammo_total = 4,
	_hud_multitexture_overlay_effector_source_weapon_heat = 5,
	_hud_multitexture_overlay_effector_source_explicit = 6,
	_hud_multitexture_overlay_effector_source_zoom_level = 7,
	NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCES = 8,
};

#endif /* HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_H */

// hud_multitexture_overlay_effector_source: DB-verified via types_enum_values $E9BECA45EF828115151A785B91DFB828
