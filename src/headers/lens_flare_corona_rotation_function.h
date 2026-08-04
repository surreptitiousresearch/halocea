#ifndef LENS_FLARE_CORONA_ROTATION_FUNCTION_H
#define LENS_FLARE_CORONA_ROTATION_FUNCTION_H

/* lens_flare_corona_rotation_function — selects how a lens-flare corona's rotation angle is derived.
 * DB-verified: types_enum_values _6222A606D995D3D7F5E14DA99592715F carries these named values verbatim. */
enum lens_flare_corona_rotation_function {
	_lens_flare_corona_rotation_function_none = 0,
	_lens_flare_corona_rotation_function_eye_in_light_space = 1,
	_lens_flare_corona_rotation_function_light_in_eye_space = 2,
	_lens_flare_corona_rotation_function_eye_to_light_in_light_space = 3,
	_lens_flare_corona_rotation_function_eye_to_light_in_eye_space = 4,
	NUMBER_OF_LENS_FLARE_CORONA_ROTATION_FUNCTIONS = 5,
};

#endif /* LENS_FLARE_CORONA_ROTATION_FUNCTION_H */
