#ifndef HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATION_H
#define HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATION_H

/* hud_multitexture_overlay_effector_destination — the `destination` field of a
 * multitexture-overlay HUD effector (which map/geometry the output modifies). */
enum hud_multitexture_overlay_effector_destination {
	_hud_multitexture_overlay_effector_destination_geometry_offset = 0,
	_hud_multitexture_overlay_effector_destination_primary_map = 1,
	_hud_multitexture_overlay_effector_destination_secondary_map = 2,
	_hud_multitexture_overlay_effector_destination_tertiary_map = 3,
	NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATIONS = 4,
};

#endif /* HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATION_H */

// hud_multitexture_overlay_effector_destination: DB-verified via types_enum_values $3CDC5DB8705D15AE5E372059FCF9A62A
