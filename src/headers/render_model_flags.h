#ifndef RENDER_MODEL_FLAGS_H
#define RENDER_MODEL_FLAGS_H

/* render_model_flags — render_model flags.
 * DB-verified: types_enum_values _2FE95C2185C9A68C11440D1974253902 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum render_model_flags {
	_render_model_immediate_bit = 0,
	_render_model_shadow_bit = 1,
	_render_model_no_planar_fog_bit = 2,
	_render_model_first_person_bit = 3,
	NUMBER_OF_RENDER_MODEL_FLAGS = 4,
};

#endif /* RENDER_MODEL_FLAGS_H */

// render_model_flags: DB-verified via types_enum_values _2FE95C2185C9A68C11440D1974253902
