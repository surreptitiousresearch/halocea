#ifndef LIGHT_DEFINITION_FLAGS_H
#define LIGHT_DEFINITION_FLAGS_H

/* light_definition_flags — bit indices for light_definition.flags.
 * Source: reference enum _F4F6DE4BD2C59DCD30225B1546A16269 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum light_definition_flags {
	_light_dynamic_bit = 0x0,
	_light_no_specular_bit = 0x1,
	_light_no_reflexive_bit = 0x2,
	_light_supersize_in_first_person_bit = 0x3,
	_light_is_first_person_flashlight_bit = 0x4,
	_light_dont_fade_active_camouflage_bit = 0x5,
	NUMBER_OF_LIGHT_DEFINITION_FLAGS = 0x6,
};

#endif /* LIGHT_DEFINITION_FLAGS_H */

// light_definition_flags: DB-verified via types_enum_values _F4F6DE4BD2C59DCD30225B1546A16269 (anonymous compiled enum, names sans leading _)
