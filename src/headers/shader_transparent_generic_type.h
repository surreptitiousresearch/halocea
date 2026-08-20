#pragma once

/* shader_transparent_generic_type — the "first map type" of a shader_transparent_generic (_shader_transparent_generic.type @ +0x2A). Sole image read is shader_get_vertex_shader_permutation @0x83755638 (biased +1 into the permutation index); kept for the field's domain documentation.
 * Source: types_enum_values $9FCCCEA89881530A1DDE69E22F470E0D (DB-verified, members verbatim in DB order). */
enum shader_transparent_generic_type {
	_shader_transparent_generic_type_2d_map = 0,
	_shader_transparent_generic_type_first_map_is_reflection_cube_map = 1,
	_shader_transparent_generic_type_first_map_is_object_centered_cube_map = 2,
	_shader_transparent_generic_type_first_map_is_viewer_centered_cube_map = 3,
	NUMBER_OF_SHADER_TRANSPARENT_GENERIC_TYPES = 4,
};

// shader_transparent_generic_type: DB-verified via types_enum_values $9FCCCEA89881530A1DDE69E22F470E0D
