#pragma once

/* clipping_plane — index into the 5-plane clip volume the weather-particle renderer culls against:
 * weather_particle_system_build_clipping_planes copies the four side planes out of the render frustum
 * and synthesizes the fifth (index 4) through camera_position + camera_forward * far_clip_distance,
 * facing back along the camera forward vector — i.e. the far plane, exactly _clip_far = 4.
 * NOTE this enum does NOT describe render_frustum.world_planes[6], whose index 4 is the NEAR plane and
 * whose far plane is index 5 (render_camera_build_frustum @0x8376BE60 builds them in that order); only
 * the 5-plane weather volume (real_plane3d clip_planes[5]) is this domain.
 * Source: types_enum_values $1614EA2A23EDB7B294B115ECFAD7E52B (DB-verified). */
enum clipping_plane {
	_clip_left = 0,
	_clip_right = 1,
	_clip_top = 2,
	_clip_bottom = 3,
	_clip_far = 4,
	NUMBER_OF_CLIPPING_PLANES = 5,
};

// clipping_plane: DB-verified via types_enum_values $1614EA2A23EDB7B294B115ECFAD7E52B
