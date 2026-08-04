#ifndef RENDER_MODEL_EFFECT_TYPE_H
#define RENDER_MODEL_EFFECT_TYPE_H

/* render_model_effect_type - value of render_model_effect.type (__int16 @ 0x00) and the
 * derived rasterizer "current model effect" state (local_model_effect_type).
 * DB-verified: types_enum_values _5BCF9DACB7D947FDA5AD14CDD5628DDD carries these values verbatim. */
enum render_model_effect_type {
	_render_model_effect_type_none = 0,
	_render_model_effect_type_active_camouflage = 1,
	_render_model_effect_type_transparent_zbuffered = 2,
	NUMBER_OF_RENDER_MODEL_EFFECT_TYPES = 3,
};

#endif /* RENDER_MODEL_EFFECT_TYPE_H */
