#ifndef SHADER_FRAMEBUFFER_BLEND_FUNCTION_H
#define SHADER_FRAMEBUFFER_BLEND_FUNCTION_H

/* shader_framebuffer_blend_function - blend function selector stored in the __int16
 * framebuffer_blend_function fields of shader/decal definitions.
 * DB-verified: types_enum_values _7FAE7D673DF8509F9B2E9AF87C383CD0 carries these values verbatim. */
enum shader_framebuffer_blend_function {
	_shader_framebuffer_blend_function_alpha_blend = 0,
	_shader_framebuffer_blend_function_multiply = 1,
	_shader_framebuffer_blend_function_double_multiply = 2,
	_shader_framebuffer_blend_function_add = 3,
	_shader_framebuffer_blend_function_reverse_subtract = 4,
	_shader_framebuffer_blend_function_min = 5,
	_shader_framebuffer_blend_function_max = 6,
	_shader_framebuffer_blend_function_alpha_multiply_add = 7,
	NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS = 8,
};

#endif /* SHADER_FRAMEBUFFER_BLEND_FUNCTION_H */
