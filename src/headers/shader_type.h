#ifndef SHADER_TYPE_H
#define SHADER_TYPE_H

/* shader_type - shader_definition base.type (shader tag class). */
enum shader_type {
	_shader_type_screen = 0,
	_shader_type_effect = 1,
	_shader_type_decal = 2,
	_shader_type_environment = 3,
	_shader_type_model = 4,
	_shader_type_transparent_generic = 5,
	_shader_type_transparent_chicago = 6,
	_shader_type_transparent_chicago_extended = 7,
	_shader_type_transparent_water = 8,
	_shader_type_transparent_glass = 9,
	_shader_type_transparent_meter = 10,
	_shader_type_transparent_plasma = 11,
	NUMBER_OF_SHADER_TYPES = 12,
};

#endif /* SHADER_TYPE_H */

// shader_type: DB-verified via types_enum_values _18716C6F6A68A0802D5937CE8A7A3251 (anonymous compiled enum, names sans leading _)
