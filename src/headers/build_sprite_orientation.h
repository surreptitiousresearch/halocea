#ifndef BUILD_SPRITE_ORIENTATION_H
#define BUILD_SPRITE_ORIENTATION_H

/* build_sprite_orientation — the `mode` argument threaded through the build_sprite_* builders,
 * selecting how a sprite's basis/scale is oriented relative to the view.
 * DB-verified: types_enum_values _4395BE2AE0F0A99CEDC71562C3F8FA6D carries these named values verbatim. */
enum build_sprite_orientation {
	_build_sprite_normal              = 0,
	_build_sprite_parallel            = 1,
	_build_sprite_perpendicular       = 2,
	NUMBER_OF_BUILD_SPRITE_ORIENTATIONS = 3,
};

#endif /* BUILD_SPRITE_ORIENTATION_H */

// build_sprite_orientation: DB-verified via types_enum_values _4395BE2AE0F0A99CEDC71562C3F8FA6D
