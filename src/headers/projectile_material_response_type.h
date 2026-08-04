#ifndef PROJECTILE_MATERIAL_RESPONSE_TYPE_H
#define PROJECTILE_MATERIAL_RESPONSE_TYPE_H

/* projectile material response codes carried in projectile_material_response_definition
 * .default_response / .possible_response.
 * DB-verified: types_enum_values _784A96263065A39EFE9E4F62C9FF0F71 carries these names verbatim. */
enum projectile_material_response_type {
	_projectile_response_disappear = 0,
	_projectile_response_detonate = 1,
	_projectile_response_reflect = 2,
	_projectile_response_penetrate = 3,
	_projectile_response_attach = 4,
	NUMBER_OF_PROJECTILE_MATERIAL_RESPONSES = 5,
};

/* projectile_material_response_definition.scale_effects_by mode.
 * DB-verified: types_enum_values _C8EFFE17236B1A6D487163438B8C3A27. */
enum projectile_material_response_scale_mode {
	_projectile_material_response_scale_effects_by_damage = 0,
	_projectile_material_response_scale_effects_by_angle = 1,
	NUMBER_OF_PROJECTILE_MATERIAL_RESPONSE_SCALE_MODES = 2,
};

#endif /* PROJECTILE_MATERIAL_RESPONSE_TYPE_H */
