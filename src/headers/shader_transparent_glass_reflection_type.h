#ifndef SHADER_TRANSPARENT_GLASS_REFLECTION_TYPE_H
#define SHADER_TRANSPARENT_GLASS_REFLECTION_TYPE_H

/* shader_transparent_glass reflection type — the _shader_transparent_glass.reflection_type field
 * (offset 0x62 in the glass body, 0x8A in the whole tag).
 * DB-verified: types_enum_values _E160613C0679F6AD80E5248BEAFC0911. */
enum shader_transparent_glass_reflection_type
{
	_shader_transparent_glass_reflection_type_bumped = 0,
	_shader_transparent_glass_reflection_type_flat = 1,
	_shader_transparent_glass_reflection_type_mirror = 2,
	NUMBER_OF_SHADER_TRANSPARENT_GLASS_REFLECTION_TYPES = 3,
};

#endif /* SHADER_TRANSPARENT_GLASS_REFLECTION_TYPE_H */
