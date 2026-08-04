#ifndef ANIMATION_FRAME_INFO_TYPE_H
#define ANIMATION_FRAME_INFO_TYPE_H

/* animation_frame_info_type — the per-frame root-motion payload layout of a model animation
 * (animation.frame_info_type).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source in this binary (no enum in
 * headers_ref/DB) — rationale: names are the published Halo CE tag-definition option list
 * ("none", "dx,dy", "dx,dy,dyaw", "dx,dy,dz,dyaw"), and the frame strides decoded in
 * biped_update_moving / biped_update_moving_simulated match each layout exactly:
 * type 1 reads 8-byte frames {dx,dy}, type 2 12-byte frames {dx,dy,dyaw},
 * type 3 16-byte frames {dx,dy,dz,dyaw}. */
/* layout bound to DB enum: types_enum_values _5F7E8954A0CEA4CA439E39A4E2ABC692 */
enum animation_frame_info_type {
	_animation_frame_info_none = 0x0,
	_animation_frame_info_xy_translation = 0x1,
	_animation_frame_info_xy_translation_yaw_rotation = 0x2,
	_animation_frame_info_xyz_translation_yaw_rotation = 0x3,
	NUMBER_OF_ANIMATION_FRAME_INFO_TYPES = 0x4,
};

#endif /* ANIMATION_FRAME_INFO_TYPE_H */
