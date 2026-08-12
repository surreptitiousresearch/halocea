#pragma once

/* shader_transparent_chicago_type — value of _shader_transparent_chicago.type (offset 0x02), the
 * "first map type" selector of a shader_transparent_chicago tag. It indexes the two 4-entry .rdata
 * tables in src/data — bitmap_type_table (2d vs cube map) and bitmap_address_table (D3DTADDRESS_WRAP
 * vs _CLAMP) — whose only consumer is rasterizer_dx9_transparent_chicago_draw @0x8382EBCC; both
 * tables already carry this enum's member names as their per-entry comments.
 * Source: types_enum_values $1587AB03BBD65F212ACF40B64423BA8C (DB-verified). */
enum shader_transparent_chicago_type {
	_shader_transparent_chicago_type_2d_map = 0,
	_shader_transparent_chicago_type_first_map_is_reflection_cube_map = 1,
	_shader_transparent_chicago_type_first_map_is_object_centered_cube_map = 2,
	_shader_transparent_chicago_type_first_map_is_viewer_centered_cube_map = 3,
	NUMBER_OF_SHADER_TRANSPARENT_CHICAGO_TYPES = 4,
};

// shader_transparent_chicago_type: DB-verified via types_enum_values $1587AB03BBD65F212ACF40B64423BA8C
